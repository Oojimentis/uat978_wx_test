// Part of dump978, a UAT decoder.
//
// Copyright 2015, Oliver Jowett <oliver@mutability.co.uk>
//

#ifndef NULL
#define NULL   ((void *) 0)
#endif

#include <math.h>
#include <string.h>
#include <assert.h>

#include <stdio.h>
#include <sqlite3.h>

#include "asprintf.h"
#include "uat.h"
#include "uat_decode.h"
#include "uat_geo.h"

#include "metar.h"
#include <time.h>

#include <stdlib.h>

static char gs_ret[80];
static char gs_ret_lat[25];
static char gs_ret_lng[25];

static void get_graphic(const struct fisb_apdu *apdu,FILE *fnm,FILE *to);
static void get_text(const struct fisb_apdu *apdu,FILE *fnm,FILE *to);
static void get_seg_text(const struct fisb_apdu *apdu,FILE *fnm,FILE *to);
static void get_gs_name(char *Word);
static void get_sua_text(char *Word,FILE *to);
static void get_pirep(char *Word,FILE *to);

// The odd two-string-literals here is to avoid \0x3ABCDEF being interpreted as a single (very large valued) character
static const char *dlac_alphabet = "\x03" "ABCDEFGHIJKLMNOPQRSTUVWXYZ\x1A\t\x1E\n| !\"#$%&'()*+,-./0123456789:;<=>?";

static const char *decode_dlac(uint8_t *data,unsigned bytelen,int rec_offset)
{
    static char buf[2048];

    char *p = buf;
    int step = 0;
    int tab = 0;

    bytelen = bytelen - rec_offset;
    uint8_t *end = data + bytelen;

    while (data < end) {
        int ch;
        assert(step >= 0 && step <= 3);
        switch (step) {
        case 0:
            ch = data[rec_offset] >> 2;
            ++data;
            break;
        case 1:
            ch = ((data[rec_offset-1] & 0x03) << 4) | (data[rec_offset] >> 4);
            ++data;
            break;
        case 2:
            ch = ((data[rec_offset-1] & 0x0f) << 2) | (data[rec_offset] >> 6);
            break;
        case 3:
            ch = data[rec_offset] & 0x3f;
            ++data;
            break;
        }
        if (tab) {
            while (ch > 0)
                *p++ = ' ',ch--;
            tab = 0;
        }
        else if (ch == 28) { // tab
            tab = 1;
        }
        else {
            *p++ = dlac_alphabet[ch];
        }
        step = (step+1)%4;
    }
    *p = 0;

    return buf;
}

static char base40_alphabet[40] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ  ..";

static double dimensions_widths[16] = {
    11.5,23,28.5,34,33,38,39.5,45,45,52,59.5,67,72.5,80,80,90
};

static const char *object_types_text[16] = {
		"Airport(or Heliport)",
		"Runway(or Helipad)",
		"Taxiway",
		"Apron",
		"Frequency Area",
		"Signage",
		"Approach Lighting",
		"Airport Lighting",
		"Obstruction OBST",
		"Construction Area",
		"Communication Equipment",
		"Navigation Equipment",
		"Surveillance Equipment",
		"Weather Equipment",
		"Airspace",
		"Future Use"
};

static const char *object_status_text[16] = {
		"Closed",
		"Closed-Conditional",
		"Arrival Only",
		"Departure Only",
		"Displaced",
		"Braking Action",
		"Obscured or Missing",
		"Unmarked",
		"Unlighted",
		"In Service",
		"Inoperative",
		"Unavailable",
		"Surface Condition",
		"Cancelled",
		"Unsafe",
		"In Effect"
};

static const char *overlay_options_text[16] = {
		"No Geometry",                              // 0
		"Low Resolution 2D Polygon",
		"High Resolution 3D Polygon",
		"Extended Range 3D Polygon (MSL)",
		"Extended Range 3D Polygon (AGL)",
		"Low Resolution 2D Ellipse",
		"High Resolution 3D Ellipse",
		"Extended Range Circular Prism (MSL)",
		"Extended Range Circular Prism (AGL)",
		"Extended Range 3D Point (AGL)",
		"Extended Range 3D Point (MSL)",                  //10
		"Extended Range 3D Polyline (MSL)",               // 11
		"Extended Range 3D Polyline (AGL)",               // 12
		"Future Use",
		"Future Use",
		"Future Use"
};

static const char *airspace_element_names[14] = {
		"Temporary Flight Restriction",
		"Parachute Jumping /Sky Diving",
		"Terminal Radar Service Area",
		"Airport Advisory Area",
		"VFR Flyway",
		"VFR Corridor",
		"VFR Transition Route",
		"Terminal Area VFR Route",
		"Prohibited Area ",
		"Restricted Area ",
		"Military Operations Area",
		"Warning Area",
		"Military Training Route",
		"Air Defense Identification Zone"
};

static const char *gairspace_element_names[16] = {
		"Temporary Flight Restriction",
		"Turbulence",
		"Low Level Wind Shear",
		"Surface Winds",
		"Icing",
		"Freezing Level",
		"IFR Conditions",
		"Mountain Obscuration",
		"Future Use",
		"Future Use",
		"Future Use",
		"Future Use",
		"Future Use",
		"Future Use",
		"Future Use",
		"Future Use"
};

static const char *obj_param_type_names[31] = {
		"Reserved",
		"Distance Nautical Miles",
		"Distance Statute Miles",
		"Length Feet",
		"Length Meters",
		"Height Feet (MSL)",
		"Height Feet (AGL)",
		"Height Meters",
		"Width Feet",
		"Width Meters",
		"Depth Fractional Inches",
		"Depth Inches",
		"Depth Feet",
		"Weight Pounds",
		"Friction Measure (MU)",
		"Direction (Magnetic)",
		"Direction (True)",
		"Visibility Feet",
		"Visibility Meters",
		"Visibility Statute Miles",
		"Speed Knots",
		"Speed Meters",
		"Frequency in MHz (25)",
		"Frequency in MHz (8.33)",
		"Future Use",
		"Future Use",
		"Future Use",
		"Future Use",
		"Future Use",
		"Future Use",
		"Future Use"
};

static const char *address_qualifier_names[8] = {
		"ICAO address via ADS-B",
		"reserved (national use)",
		"ICAO address via TIS-B",
		"TIS-B track file address",
		"Vehicle address",
		"Fixed ADS-B Beacon Address",
		"reserved (6)",
		"reserved (7)"
};

static const char *emitter_category_names[40] = {

		"No aircraft type information",
		"Light < 15 500 lbs",
		"Small – 15,500 to 75,000 lbs",
		"Large – 75,000 to 300,000 lbs",
		"High Vortex Large",
		"Heavy - > 300,000 lbs",
		"Highly Maneuverable",
		"Rotocraft",
		"Unassigned 8",
		"Glider/sailplane",
		"Lighter than air",
		"Parachutist/sky diver",
		"Ultra light/hang glider/paraglider",
		"Unassigned 13",
		"Unmanned aerial vehicle",
		"Space/trans-atmospheric vehicle",
		"Unassigned 16",
		"Surface vehicle—emergency vehicle",
		"Surface vehicle—service vehicle",
		"Point Obstacle (includes tethered balloons)",
		"Cluster Obstacle"
		"Line Obstacle",
        "reserved (22)",
		"reserved (23)",                           // C7
		"reserved (24)",
		"reserved (25)",
		"reserved (26)",
    	"reserved (27)",
    	"reserved (28)",
    	"reserved (29)",
    	"reserved (30)",
    	"reserved (31)",
    	"reserved (32)",
    	"reserved (33)",
    	"reserved (34)",
    	"reserved (35)",
    	"reserved (36)",
    	"reserved (37)",
    	"reserved (38)",
    	"reserved (39)"
};

static const char *emergency_status_names[8] = {

		"No emergency",
		"**** General emergency ****",
		"**** Lifeguard / Medical emergency ****",
		"**** Minimum fuel ****",
		"**** No communications ****",
		"**** Unlawful interference ****",
		"**** Downed aircraft ****",
		"reserved"
};

static const char *info_frame_type_names[16] = {

		"FIS-B APDU",
		"Reserved for Developmental Use",
		"Reserved for Future Use (2)",
		"Reserved for Future Use (3)",
		"Reserved for Future Use (4)",
		"Reserved for Future Use (5)",
		"Reserved for Future Use (6)",
		"Reserved for Future Use (7)",
		"Reserved for Future Use (8)",
		"Reserved for Future Use (9)",
		"Reserved for Future Use (10)",
		"Reserved for Future Use (11)",
		"Reserved for Future Use (12)",
		"Reserved for Future Use (13)",
		"Current Report List (14)    ",
		"TIS-B/ADS-R Service Status"
};

static int stn_callback(void *data, int argc, char **argv, char **azColName){
//   Station details from database

	strcpy(gs_ret,argv[1]);
	sprintf(gs_ret_lat, "%s", argv[2]);
	sprintf(gs_ret_lng, "%s", argv[3]);
	return 0;
}

static void get_gs_name(char *Word)
{												// Get ground station data
	char temp_stn[5] = " ";
	char *sql;
	char *zErrMsg = 0;

	strncpy(temp_stn,Word,4);
	strcpy(gs_ret,"not found      ");
	strcpy(gs_ret_lat,"0.0");
	strcpy(gs_ret_lng,"0.0");

	asprintf(&sql,"SELECT * from stations where stn_call = '%s'",temp_stn);

	rc = sqlite3_exec(db, sql, stn_callback, 0, &zErrMsg);
	if( rc != SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	return;
}

static void get_sua_text(char *Word,FILE *to)
{													// SUA Decode
	char *token;
	char sua_sch_id[10];
	char sua_aspc_id[7];
	char sua_sch_stat[2];
	char sua_aspc_ty[2];
	char sua_aspc_nm[50];

	char sua_st_tm[11]; 	char sua_st_yy[3]; 	char sua_st_mm[3]; char sua_st_dd[3];
	char sua_st_hh[3];  	char sua_st_mn[3];
	char sua_en_tm[11]; 	char sua_en_yy[3]; 	char sua_en_mm[3]; char sua_en_dd[3];
	char sua_en_hh[3];  	char sua_en_mn[3];

	char sua_low_alt[10];	char sua_hg_alt[10];
	char sua_sep_rl[2];
	char sua_shp_ind[2];
	char sua_nfdc_id[10];	char sua_nfcd_nm[50];
	char sua_dafif_id[10];	char sua_dafif_nm[50];

	token = strsep(&Word,"|");
	strcpy(sua_sch_id,token);
	fprintf(filesua," Schedule ID     : %s ",sua_sch_id);
	token = strsep(&Word,"|");
	strcpy(sua_aspc_id,token);
	fprintf(filesua,"     Airspace ID       : %s\n",sua_aspc_id);
	token = strsep(&Word,"|");
	strcpy(sua_sch_stat,token);

	if (strcmp(sua_sch_stat,"W") == 0){
		fprintf(filesua," Schedule Status : %s Waiting to Start\n",sua_sch_stat);}
	else if (strcmp(sua_sch_stat,"P") == 0){
		fprintf(filesua," Schedule Status : %s Pending Approval\n",sua_sch_stat);}
	else if (strcmp(sua_sch_stat,"H") == 0){
		fprintf(filesua," Schedule Status : %s Activated for Use\n",sua_sch_stat);}

	token = strsep(&Word,"|");
	strcpy(sua_aspc_ty,token);

	if (strcmp(sua_aspc_ty,"W") == 0){
		fprintf(filesua," Airspace Type   : %s Warning Area\n",sua_aspc_ty);}
	else if (strcmp(sua_aspc_ty,"R") == 0){
		fprintf(filesua," Airspace Type   : %s Restricted Area\n",sua_aspc_ty);}
	else if (strcmp(sua_aspc_ty,"M") == 0){
		fprintf(filesua," Airspace Type   : %s Military Operations Area\n",sua_aspc_ty);}
	else if (strcmp(sua_aspc_ty,"P") == 0){
		fprintf(filesua," Airspace Type   : %s Prohibited Area\n",sua_aspc_ty);}
	else if (strcmp(sua_aspc_ty,"L") == 0){
		fprintf(filesua," Airspace Type   : %s Alert Area\n",sua_aspc_ty);}
	else if (strcmp(sua_aspc_ty,"A") == 0){
		fprintf(filesua," Airspace Type   : %s ATCAA\n",sua_aspc_ty);}
	else if (strcmp(sua_aspc_ty,"I") == 0){
		fprintf(filesua," Airspace Type   : %s Instrument Route\n",sua_aspc_ty);}
	else if (strcmp(sua_aspc_ty,"V") == 0){
		fprintf(filesua," Airspace Type   : %s Visual Route\n",sua_aspc_ty);}
	else if (strcmp(sua_aspc_ty,"S") == 0){
		fprintf(filesua," Airspace Type   : %s Slow Route\n",sua_aspc_ty);}
	else if (strcmp(sua_aspc_ty,"B") == 0){
		fprintf(filesua," Airspace Type   : %s Military Route (Refueling)\n",sua_aspc_ty);}
	else if (strcmp(sua_aspc_ty,"O") == 0){
		fprintf(filesua," Airspace Type   : %s Other\n",sua_aspc_ty);}
	else if (strcmp(sua_aspc_ty,"T") == 0){
		fprintf(filesua," Airspace Type   : %s Refueling Track\n",sua_aspc_ty);}

	token = strsep(&Word,"|");
	strcpy(sua_aspc_nm,token);
	fprintf(filesua," Airspace Name   : %s\n",sua_aspc_nm);
	token = strsep(&Word,"|");
	strcpy(sua_st_tm,token);
	snprintf(sua_st_yy,3,"%s",sua_st_tm);
	snprintf(sua_st_mm,3,"%s",sua_st_tm+2);
	snprintf(sua_st_dd,3,"%s",sua_st_tm+4);
	snprintf(sua_st_hh,3,"%s",sua_st_tm+6);
	snprintf(sua_st_mn,3,"%s",sua_st_tm+8);

	fprintf(filesua," Start Time      : %s/%s/20%s %s:%s",sua_st_mm,sua_st_dd,sua_st_yy,sua_st_hh,sua_st_mn);

	token = strsep(&Word,"|");
	strcpy(sua_en_tm,token);
	snprintf(sua_en_yy,3,"%s",sua_en_tm);
	snprintf(sua_en_mm,3,"%s",sua_en_tm+2);
	snprintf(sua_en_dd,3,"%s",sua_en_tm+4);
	snprintf(sua_en_hh,3,"%s",sua_en_tm+6);
	snprintf(sua_en_mn,3,"%s",sua_en_tm+8);

	fprintf(filesua," End Time: %s/%s/20%s %s:%s\n",sua_en_mm,sua_en_dd,sua_en_yy,sua_en_hh,sua_en_mn);

	token = strsep(&Word,"|");
	strcpy(sua_low_alt,token);
	fprintf(filesua," Low Altitude    : %s ",sua_low_alt);
	token = strsep(&Word,"|");
	strcpy(sua_hg_alt,token);
	fprintf(filesua," High Altitude: %s\n",sua_hg_alt);
	token = strsep(&Word,"|");
	strcpy(sua_sep_rl,token);
	if (strcmp(sua_sep_rl,"A") == 0){
		fprintf(filesua," Separation Rule : %s Aircraft Rule ",sua_sep_rl);}
	else if (strcmp(sua_sep_rl,"O") == 0){
		fprintf(filesua," Separation Rule : %s Other Rule ",sua_sep_rl);}
	else {
		fprintf(filesua," Separation Rule :    Unspecified ");
	}
	token = strsep(&Word,"|");
	if (token){
		strcpy(sua_shp_ind,token);     	//13
		if (strcmp(sua_shp_ind,"N") == 0){
			fprintf(filesua," Shape Indicator: %s No Shape Defined\n",sua_shp_ind);}
		else if	(strcmp(sua_shp_ind,"Y") == 0){
			fprintf(filesua," Shape Indicator: %s Has Shape Defined\n",sua_shp_ind);}
	}
	token = strsep(&Word,"|");
	if (token){
		strcpy(sua_nfdc_id,token);      //14
		fprintf(filesua," NFDC ID         : %10s",sua_nfdc_id);
	}
	token = strsep(&Word,"|");
	if (token) {
		strcpy(sua_nfcd_nm,token);      //15
		fprintf(filesua,"      NFCD Name : %s\n",sua_nfcd_nm);
	}
	token = strsep(&Word,"|");
	if (token) {
		strcpy(sua_dafif_id,token);    	//16
		fprintf(filesua," DAFIF ID        : %10s",sua_dafif_id);
	}
	token = strsep(&Word,"|");
	if (token) {
		strcpy(sua_dafif_nm,token);    	//17
		fprintf(filesua,"      DAFIF Name: %s\n",sua_dafif_nm);
	}
	fflush(filesua);
}

static void get_pirep(char *Word,FILE *to)
{
    char *token;
    char pirep_stn[5];
    char pirep_OV[30];										// Location
    char pirep_TM[10]; char pirep_hr[5]; char pirep_mn[3];	// DateTime
    char pirep_FL[10];										// Flight Level
    char pirep_TP[10];										// a/c type
    char pirep_SK[50];										// Cloud
    char pirep_WX[30];										// Weather
    char pirep_TA[10];										// Temperature
    char pirep_WV[15];										// Wind Speed Direction
    char pirep_TB[30];  									// Turbulence
    char pirep_IC[30];										// Icing
    char pirep_RM[100]; 									// Remarks

	token = strtok(Word," ");
	strcpy(pirep_stn,"K");
	strcat(pirep_stn,token);

    get_gs_name(pirep_stn);

    fprintf(filepirep,"PIREP REPORT:\n");

    time_t current_time = time(NULL);
 	char * tm=ctime(&current_time);
    tm[strlen(tm)-1] = '\0';
	fprintf(filepirep," Time           : %s\n",tm);
    fprintf(filepirep," Station        : %s - %s\n",pirep_stn,gs_ret);

    token = strtok(0," ");
    if( strcmp(token,"UUA") == 0 ){
    	fprintf(filepirep," URGENT REPORT\n");
    }
    else if  ( strcmp(token,"UA") == 0 ){
    	fprintf(filepirep," Routine Report\n");
    }
    else {
    	fprintf(filepirep," Unknown Report\n");
    }
    while ((token = strtok(0,"/"))) {
    	if (strncmp(token,"OV",2) == 0) {
    		strcpy(pirep_OV,token+3);
    		fprintf(filepirep," Location       : %s\n",pirep_OV);
    	 }
    	 else if (strncmp(token,"TM",2) == 0) {
    		 strcpy(pirep_TM,token);
    		 snprintf(pirep_hr,3,"%s",pirep_TM+3);
    		 snprintf(pirep_mn,3,"%s",pirep_TM+5);
    		 fprintf(filepirep," Time           : %s:%sz\n",pirep_hr,pirep_mn);
    	 }
    	 else if (strncmp(token,"FL",2) == 0) {
    		 strcpy(pirep_FL,token+2);
    	     fprintf(filepirep," Flight Level   : %s\n",pirep_FL);
    	 }
    	 else if (strncmp(token,"TP",2) == 0) {
    		 strcpy(pirep_TP,token+3);
    		 fprintf(filepirep," A/C Type       : %s\n",pirep_TP);
    	 }
    	 else if (strncmp(token,"SK",2) == 0) {
    		 strcpy(pirep_SK,token+3);
    	     fprintf(filepirep," Cloud Layers   : %s\n",pirep_SK);
    	 }
    	 else if (strncmp(token,"WX",2) == 0) {
 	     	 strcpy(pirep_WX,token+3);
    	     fprintf(filepirep," Weather        : %s\n",pirep_WX);
    	 }
       	 else if (strncmp(token,"TA",2) == 0) {
    	     strcpy(pirep_TA,token+3);
        	 fprintf(filepirep," Temperature    : %s(c)\n",pirep_TA);
       	 }
         else if (strncmp(token,"WV",2) == 0) {
        	 strcpy(pirep_WV,token+3);
             fprintf(filepirep," WndSpdDir      : %s\n",pirep_WV);
         }
         else if (strncmp(token,"TB",2) == 0) {
    		 strcpy(pirep_TB,token+3);
    	     fprintf(filepirep," Turbulence     : %s\n",pirep_TB);
         }
         else if (strncmp(token,"IC",2) == 0) {
    		 strcpy(pirep_IC,token+3);
    	     fprintf(filepirep," Icing          : %s\n",pirep_IC);
         }
    	 else if (strncmp(token,"RM",2) == 0) {
    		 strcpy(pirep_RM,token+3);
    		 token = strtok(0,"/");
    		 if (token){
    			 fprintf(filepirep," Remarks        : %s",pirep_RM);
    			 fprintf(filepirep,"/%s\n",token);}
    		 else {
    			 fprintf(filepirep," Remarks        : %s\n",pirep_RM);
    		 }
    	 }
    }
    fflush(filepirep);
}

static void uat_decode_hdr(uint8_t *frame,struct uat_adsb_mdb *mdb)
{
    mdb->mdb_type = (frame[0] >> 3) & 0x1f;
    mdb->address_qualifier = (address_qualifier_t) (frame[0] & 0x07);
    mdb->address = (frame[1] << 16) | (frame[2] << 8) | frame[3];
}

static void uat_display_hdr(const struct uat_adsb_mdb *mdb,FILE *to)
{
	fprintf(to,"HDR:");

	time_t current_time = time(NULL);
	struct tm *tm = localtime(&current_time);
	fprintf(to,"   Time: %s",asctime(tm));
	fprintf(to," ICAO:    %06X    (%s)\n",mdb->address,address_qualifier_names[mdb->address_qualifier]);
}

static void uat_decode_sv(uint8_t *frame,struct uat_adsb_mdb *mdb)
{
    uint32_t raw_lat;
	uint32_t raw_lon;
	uint32_t raw_alt;

   	mdb->has_sv = 1;
    mdb->nic = (frame[11] & 15);

    raw_lat = (frame[4] << 15) | (frame[5] << 7) | (frame[6] >> 1);
    raw_lon = ((frame[6] & 0x01) << 23) | (frame[7] << 15) | (frame[8] << 7) | (frame[9] >> 1);
    
    if (mdb->nic != 0 || raw_lat != 0 || raw_lon != 0) {
        mdb->position_valid = 1;
        mdb->lat = raw_lat * 360.0 / 16777216.0;
        if (mdb->lat > 90)
            mdb->lat -= 180;
        mdb->lon = raw_lon * 360.0 / 16777216.0;
        if (mdb->lon > 180)
            mdb->lon -= 360;
    }
    raw_alt = (frame[10] << 4) | ((frame[11] & 0xf0) >> 4);
    if (raw_alt != 0) {
        mdb->altitude_type = (frame[9] & 1) ? ALT_GEO : ALT_BARO;
        mdb->altitude = (raw_alt - 1) * 25 - 1000;
    }
    mdb->airground_state = (frame[12] >> 6) & 0x03;

    switch (mdb->airground_state) {
    case AG_SUBSONIC:
    case AG_SUPERSONIC:
    {
    	int raw_ns,raw_ew,raw_vvel;
            
    	raw_ns = ((frame[12] & 0x1f) << 6) | ((frame[13] & 0xfc) >> 2);

    	if ((raw_ns & 0x3ff) != 0) {
    		mdb->ns_vel_valid = 1;
    		mdb->ns_vel = ((raw_ns & 0x3ff) - 1);
    		if (raw_ns & 0x400)
    			mdb->ns_vel = 0 - mdb->ns_vel;
    		if (mdb->airground_state == AG_SUPERSONIC)
    			mdb->ns_vel *= 4;
            }
    	raw_ew = ((frame[13] & 0x03) << 9) | (frame[14] << 1) | ((frame[15] & 0x80) >> 7);

    	if ((raw_ew & 0x3ff) != 0) {
    		mdb->ew_vel_valid = 1;
    		mdb->ew_vel = ((raw_ew & 0x3ff) - 1);
    		if (raw_ew & 0x400)
    			mdb->ew_vel = 0 - mdb->ew_vel;
    		if (mdb->airground_state == AG_SUPERSONIC)
                    mdb->ew_vel *= 4;
            }
            if (mdb->ns_vel_valid && mdb->ew_vel_valid) {
            	if (mdb->ns_vel != 0 || mdb->ew_vel != 0) {
            		mdb->track_type = TT_TRACK;
                    mdb->track = (uint16_t)(360 + 90 - atan2(mdb->ns_vel,mdb->ew_vel) * 180 / M_PI) % 360;
                }
                mdb->speed_valid = 1;
                mdb->speed = (int) sqrt(mdb->ns_vel * mdb->ns_vel + mdb->ew_vel * mdb->ew_vel);
            }
            raw_vvel = ((frame[15] & 0x7f) << 4) | ((frame[16] & 0xf0) >> 4);
            if ((raw_vvel & 0x1ff) != 0) {
                mdb->vert_rate_source = (raw_vvel & 0x400) ? ALT_BARO : ALT_GEO;
                mdb->vert_rate = ((raw_vvel & 0x1ff) - 1) * 64;
                if (raw_vvel & 0x200)
                    mdb->vert_rate = 0 - mdb->vert_rate;
            }                
        }
        break;

    case AG_GROUND:
    {
    	int raw_gs,raw_track;

    	raw_gs = ((frame[12] & 0x1f) << 6) | ((frame[13] & 0xfc) >> 2);
    	if (raw_gs != 0) {
    		mdb->speed_valid = 1;
    		mdb->speed = ((raw_gs & 0x3ff) - 1);
    	}
    	raw_track = ((frame[13] & 0x03) << 9) | (frame[14] << 1) | ((frame[15] & 0x80) >> 7);

    	switch ((raw_track & 0x0600)>>9) {
    	case 1: mdb->track_type = TT_TRACK; break;
    	case 2: mdb->track_type = TT_MAG_HEADING; break;
    	case 3: mdb->track_type = TT_TRUE_HEADING; break;
    	}

    	if (mdb->track_type != TT_INVALID)
    		mdb->track = (raw_track & 0x1ff) * 360 / 512;

    	mdb->dimensions_valid = 1;
    	mdb->length = 15 + 10 * ((frame[15] & 0x38) >> 3);
    	mdb->width = dimensions_widths[(frame[15] & 0x78) >> 3];
    	mdb->position_offset = (frame[15] & 0x04) ? 1 : 0;
    }
    break;

    case AG_RESERVED:
        // nothing
    	break;
    }
    
    if ((frame[0] & 7) == 2 || (frame[0] & 7) == 3) {
        mdb->utc_coupled = 0;
        mdb->tisb_site_id = (frame[16] & 0x0f);
    }
    else {
        mdb->utc_coupled = (frame[16] & 0x08) ? 1 : 0;
        mdb->tisb_site_id = 0;
    }
}

static void uat_display_sv(const struct uat_adsb_mdb *mdb,FILE *to)
{
	if (!mdb->has_sv)
        return;
    if (mdb->position_valid)
        fprintf(to," Lat:   %+.4f    Lon:  %+.4f",mdb->lat,mdb->lon);

    switch (mdb->altitude_type) {
    case ALT_BARO:
        fprintf(to,"  Alt:  %d ft (barometric)",mdb->altitude);
        break;
    case ALT_GEO:
        fprintf(to,"  Alt:  %d ft (geometric)",mdb->altitude);
        break;
    default:
        break;
    }

    if (mdb->speed_valid)
        fprintf(to,"  Speed:  %u kt\n",mdb->speed);
    if (mdb->dimensions_valid)
        fprintf(to," Size: %.1fm L x %.1fm W%s\n",mdb->length,mdb->width,
                mdb->position_offset ? " (position offset applied)" : "");
}

static void uat_decode_ms(uint8_t *frame,struct uat_adsb_mdb *mdb)
{
    uint16_t v;
    int i;

    mdb->has_ms = 1;

    v = (frame[17]<<8) | (frame[18]);
    mdb->emitter_category = (v/1600) % 40;
    mdb->callsign[0] = base40_alphabet[(v/40) % 40];
    mdb->callsign[1] = base40_alphabet[v % 40];
    v = (frame[19]<<8) | (frame[20]);
    mdb->callsign[2] = base40_alphabet[(v/1600) % 40];
    mdb->callsign[3] = base40_alphabet[(v/40) % 40];
    mdb->callsign[4] = base40_alphabet[v % 40];
    v = (frame[21]<<8) | (frame[22]);
    mdb->callsign[5] = base40_alphabet[(v/1600) % 40];
    mdb->callsign[6] = base40_alphabet[(v/40) % 40];
    mdb->callsign[7] = base40_alphabet[v % 40];
    mdb->callsign[8] = 0;

    // trim trailing spaces
    for (i = 7; i >= 0; --i) {
        if (mdb->callsign[i] == ' ')
            mdb->callsign[i] = 0;
        else
            break;
    }

    mdb->emergency_status 	= (frame[23] >> 5) & 7;
    mdb->uat_version		= (frame[23] >> 2) & 7;
    mdb->sil 				= (frame[23] & 3);
    mdb->transmit_mso 		= (frame[24] >> 2) & 0x3f;
    mdb->nac_p 				= (frame[25] >> 4) & 15;
    mdb->nac_v 				= (frame[25] >> 1) & 7;
    mdb->nic_baro 			= (frame[25] & 1);
    mdb->has_cdti 			= (frame[26] & 0x80 ? 1 : 0);
    mdb->has_acas 			= (frame[26] & 0x40 ? 1 : 0);
    mdb->acas_ra_active 	= (frame[26] & 0x20 ? 1 : 0);
    mdb->ident_active 		= (frame[26] & 0x10 ? 1 : 0);
    mdb->atc_services 		= (frame[26] & 0x08 ? 1 : 0);
    mdb->heading_type 		= (frame[26] & 0x04 ? HT_MAGNETIC : HT_TRUE);

    if (mdb->callsign[0])
        mdb->callsign_type = (frame[26] & 0x02 ? CS_CALLSIGN : CS_SQUAWK);
}

static void uat_display_ms(const struct uat_adsb_mdb *mdb,FILE *to)
{
    if (!mdb->has_ms)
        return;

    fprintf(to," Reg:%2s%9s    Category: %s\n Emergency status:  %s\n",
            mdb->callsign_type == CS_SQUAWK ? "sq" : "",
            mdb->callsign_type == CS_INVALID ? "      n/a" : mdb->callsign,
            emitter_category_names[mdb->emitter_category],
            emergency_status_names[mdb->emergency_status]);
}

static void uat_decode_auxsv(uint8_t *frame,struct uat_adsb_mdb *mdb)
{
    int raw_alt = (frame[29] << 4) | ((frame[30] & 0xf0) >> 4);

    if (raw_alt != 0) {
        mdb->sec_altitude = (raw_alt - 1) * 25 - 1000;
        mdb->sec_altitude_type = (frame[9] & 1) ? ALT_BARO : ALT_GEO;
    }
    else {
        mdb->sec_altitude_type = ALT_INVALID;
    }
    mdb->has_auxsv = 1;
}    

static void uat_display_auxsv(const struct uat_adsb_mdb *mdb,FILE *to)
{
    if (!mdb->has_auxsv)
        return;
}

void uat_decode_adsb_mdb(uint8_t *frame,struct uat_adsb_mdb *mdb)
{
    static struct uat_adsb_mdb mdb_zero;

    *mdb = mdb_zero;

    uat_decode_hdr(frame,mdb);

    switch (mdb->mdb_type) {
    case 0: 	// HDR SV
    case 4: 	// HDR SV (TC+0) (TS)
    case 7: 	// HDR SV reserved
    case 8: 	// HDR SV reserved
    case 9: 	// HDR SV reserved
    case 10: 	// HDR SV reserved
        uat_decode_sv(frame,mdb);
        break;

    case 1: 	// HDR SV MS AUXSV
        uat_decode_sv(frame,mdb);
        uat_decode_ms(frame,mdb);
        uat_decode_auxsv(frame,mdb);
        break;

    case 2: 	// HDR SV AUXSV
    case 5: 	// HDR SV (TC+1) AUXSV
    case 6: 	// HDR SV (TS) AUXSV
        uat_decode_sv(frame,mdb);
        uat_decode_auxsv(frame,mdb);
        break;

    case 3: 	// HDR SV MS (TS)
        uat_decode_sv(frame,mdb);
        uat_decode_ms(frame,mdb);
        break;

    default:
        break;
    }
}

void uat_display_adsb_mdb(const struct uat_adsb_mdb *mdb,FILE *to)
{
    uat_display_hdr(mdb,to);
    uat_display_sv(mdb,to);
    uat_display_ms(mdb,to);
    uat_display_auxsv(mdb,to);
}

static void uat_decode_info_frame(struct uat_uplink_info_frame *frame)
{
    unsigned t_opt;

    frame->is_fisb = 0;

    if (frame->type != 0)
        return; // not FIS-B

    if (frame->length < 4) // too short for FIS-B
        return;

    t_opt = ((frame->data[1] & 0x01) << 1) | (frame->data[2] >> 7);

    switch (t_opt) {
    case 0: // Hours,Minutes
        frame->fisb.monthday_valid = 0;
        frame->fisb.seconds_valid  = 0;
        frame->fisb.hours	= (frame->data[2] & 0x7c) >> 2;
        frame->fisb.minutes = ((frame->data[2] & 0x03) << 4) | (frame->data[3] >> 4);
        frame->fisb.length  = frame->length - 4;
        frame->fisb.data    = frame->data + 4;
        break;
    case 1: // Hours,Minutes,Seconds
        if (frame->length < 5)
            return;
        frame->fisb.monthday_valid = 0;
        frame->fisb.seconds_valid  = 1;
        frame->fisb.hours   = (frame->data[2] & 0x7c) >> 2;
        frame->fisb.minutes = ((frame->data[2] & 0x03) << 4) | (frame->data[3] >> 4);
        frame->fisb.seconds = ((frame->data[3] & 0x0f) << 2) | (frame->data[4] >> 6);
        frame->fisb.length  = frame->length - 5;
        frame->fisb.data    = frame->data + 5;
        break;
    case 2: // Month,Day,Hours,Minutes
        if (frame->length < 5)
            return;
        frame->fisb.monthday_valid = 1;
        frame->fisb.seconds_valid  = 0;
        frame->fisb.month   = (frame->data[2] & 0x78) >> 3;
        frame->fisb.day     = ((frame->data[2] & 0x07) << 2) | (frame->data[3] >> 6);
        frame->fisb.hours   = (frame->data[3] & 0x3e) >> 1;
        frame->fisb.minutes = ((frame->data[3] & 0x01) << 5) | (frame->data[4] >> 3);

        if (frame->data[1] & 0x02){
            frame->fisb.length = frame->length ; // ???
            frame->fisb.data   = frame->data ; }
        else {
        frame->fisb.length = frame->length - 5; // ???
        frame->fisb.data   = frame->data + 5;
        }
        break;
    case 3: // Month,Day,Hours,Minutes,Seconds
        if (frame->length < 6)
            return;
        frame->fisb.monthday_valid = 1;
        frame->fisb.seconds_valid  = 1;
        frame->fisb.month 	= (frame->data[2] & 0x78) >> 3;
        frame->fisb.day 	= ((frame->data[2] & 0x07) << 2) | (frame->data[3] >> 6);
        frame->fisb.hours   = (frame->data[3] & 0x3e) >> 1;
        frame->fisb.minutes = ((frame->data[3] & 0x01) << 5) | (frame->data[4] >> 3);
        frame->fisb.seconds = ((frame->data[4] & 0x03) << 3) | (frame->data[5] >> 5);
        frame->fisb.length 	= frame->length - 6;
        frame->fisb.data 	= frame->data + 6;
        break;
    }
    frame->fisb.a_flag = (frame->data[0] & 0x80) ? 1 : 0;
    frame->fisb.g_flag = (frame->data[0] & 0x40) ? 1 : 0;
    frame->fisb.p_flag = (frame->data[0] & 0x20) ? 1 : 0;
    frame->fisb.product_id = ((frame->data[0] & 0x1f) << 6) | (frame->data[1] >> 2);
    frame->fisb.s_flag 	   = (frame->data[1] & 0x02) ? 1 : 0;
    frame->is_fisb = 1;
}

void uat_decode_uplink_mdb(uint8_t *frame,struct uat_uplink_mdb *mdb)
{
    mdb->position_valid = (frame[5] & 0x01) ? 1 : 0;

    /* Even with position_valid = 0,there seems to be plausible data here.
     * Decode it always.
    if (mdb->position_valid)*/

    uint32_t raw_lat = (frame[0] << 15) | (frame[1] << 7) | (frame[2] >> 1);
    uint32_t raw_lon = ((frame[2] & 0x01) << 23) | (frame[3] << 15) | (frame[4] << 7) | (frame[5] >> 1);
        
    mdb->lat = raw_lat * 360.0 / 16777216.0;
    if (mdb->lat > 90)
    	mdb->lat -= 180;

    mdb->lon = raw_lon * 360.0 / 16777216.0;
    if (mdb->lon > 180)
    	mdb->lon -= 360;

    mdb->utc_coupled 	= (frame[6] & 0x80) ? 1 : 0;
    mdb->app_data_valid = (frame[6] & 0x20) ? 1 : 0;
    mdb->slot_id 	    = (frame[6] & 0x1f);
    mdb->tisb_site_id 	= (frame[7] >> 4);

    if (mdb->app_data_valid) {
        uint8_t *data,*end;

        memcpy(mdb->app_data,frame+8,424);
        mdb->num_info_frames = 0;
        
        data = mdb->app_data;
        end  = mdb->app_data + 424;

        while (mdb->num_info_frames < UPLINK_MAX_INFO_FRAMES && data+2 <= end) {
        	struct uat_uplink_info_frame *frame = &mdb->info_frames[mdb->num_info_frames];
            frame->length = (data[0] << 1) | (data[1] >> 7);
            frame->type   = (data[1] & 0x0f);
            if (data + frame->length + 2 > end) {
                // overrun?
                break;
            }
            if (frame->length == 0 && frame->type == 0) {
                break; // no more frames
            }
            frame->data = data + 2;

            uat_decode_info_frame(frame);

            data += frame->length + 2;
            ++mdb->num_info_frames;
        }
    }
}

static void display_generic_data(uint8_t *data,uint16_t length,FILE *to)
{
    unsigned i;

    fprintf(to," Data:              ");
    for (i = 0; i < length; i += 16) {
        unsigned j;
        
        if (i > 0)
            fprintf(to,"                    ");
        for (j = i; j < i+16; ++j) {
        	if (j < length)
                fprintf(to,"%02X ",data[j]);
            else
                fprintf(to,"   ");
        }
        for (j = i; j < i+16 && j < length; ++j) {
            fprintf(to,"%c",(data[j] >= 32 && data[j] < 127) ? data[j] : '.');
        }
        fprintf(to,"\n");
    }
}

static const char *get_fisb_product_name(uint16_t product_id)
{
    switch (product_id) {
    case 0:  	case 20: return "METAR and SPECI ****";
    case 1:  	case 21: return "TAF and Amended TAF ****";
    case 2:  	case 22: return "SIGMET ****";
    case 3:  	case 23: return "Convective SIGMET ****";
    case 4:  	case 24: return "AIRMET ****";
    case 5:  	case 25: return "PIREP ****";
    case 6:  	case 26: return "AWW ****";
    case 7:  	case 27: return "Winds and Temperatures Aloft ****";
    case 8:    	return "NOTAM (Including TFRs) and Service Status";
    case 9:    	return "Aerodrome and Airspace – D-ATIS ****";
    case 10:	return "Aerodrome and Airspace - TWIP ****";
    case 11:   	return "Aerodrome and Airspace - AIRMET";
    case 12:   	return "Aerodrome and Airspace - SIGMET/Convective SIGMET";
    case 13:   	return "Aerodrome and Airspace - SUA Status";
    case 14:   	return "G-AIRMET";
    case 15:   	return "Center Weather Advisory (CWAG)";
    case 51:   	return "National NEXRAD,Type 0 - 4 Level ****";
    case 52:   	return "National NEXRAD,Type 1 - 8 Level (Quasi 6-Level VIP) ****";
    case 53:   	return "National NEXRAD,Type 2 - 8 Level ****";
    case 54:   	return "National NEXRAD,Type 3 - 16 Level ****";
    case 55:   	return "Regional NEXRAD,Type 0 - Low Dynamic Range ****";
    case 56:   	return "Regional NEXRAD,Type 1 - 8 Level (Quasi 6-Level VIP) ****";
    case 57:   	return "Regional NEXRAD,Type 2 - 8 Level ****";
    case 58:   	return "Regional NEXRAD,Type 3 - 16 Level ****";
    case 59:   	return "Individual NEXRAD,Type 0 - Low Dynamic Range ****";
    case 60:   	return "Individual NEXRAD,Type 1 - 8 Level (quasi 6-Level VIP) ****";
    case 61:   	return "Individual NEXRAD,Type 2 - 8 Level ****";
    case 62:   	return "Individual NEXRAD,Type 3 - 16 Level ****";
    case 63:   	return "Global Block Representation - Regional NEXRAD,Type 4 - 8 Level";
    case 64:   	return "Global Block Representation - CONUS NEXRAD,Type 4 - 8 Level";
    case 70:   	return "Icing Forecast - Low";
    case 71:   	return "Icing Forecast - High";
    case 81:   	return "Radar Echo Tops Graphic,Scheme 1: 16-Level ****";
    case 82:   	return "Radar Echo Tops Graphic,Scheme 2: 8-Level ****";
    case 83:   	return "Storm Tops and Velocity ****";
    case 84:   	return "Cloud Tops";
    case 90:   	return "Turbulence Forecast - Low";
    case 91:   	return "Turbulence Forecast - High";
    case 101:  	return "Lightning Strike Type 1 (Pixel Level) ****";
    case 102:  	return "Lightning Strike Type 2 (Grid Element Level) ****";
    case 103:  	return "Lightning";
    case 151:  	return "Point Phenomena,Vector Format ****";
    case 201:  	return "Surface Conditions/Winter Precipitation Graphic ****";
    case 202:  	return "Surface Weather Systems ****";
    case 254:  	return "AIRMET,SIGMET: Bitmap Encoding ****";
    case 351:  	return "System Time ****";
    case 352:  	return "Operational Status ****";
    case 353:	return "Ground Station Status ****";
    case 401:  	return "**** Generic Raster Scan Data Product APDU Payload Format Type 1";
    case 402:  	case 411: return "**** Generic Textual Data Product APDU Payload Format Type 1";
    case 403:  	return "**** Generic Vector Data Product APDU Payload Format Type 1";
    case 404:  	case 412: return "**** Generic Symbolic Product APDU Payload Format Type 1";
    case 405:  	case 413: return "Generic Textual Data Product APDU Payload Format Type 2";
    case 600:  	return "**** FISDL Products – Proprietary Encoding";
    case 2000: 	return "**** FAA/FIS-B Product 1 – Developmental";
    case 2001: 	return "**** FAA/FIS-B Product 2 – Developmental";
    case 2002: 	return "**** FAA/FIS-B Product 3 – Developmental";
    case 2003: 	return "****FAA/FIS-B Product 4 – Developmental";
    case 2004: 	return "****WSI Products - Proprietary Encoding";
    case 2005: 	return "****WSI Developmental Products";
    default: 	return "****Unknown";
    }
}

static const char *get_fisb_product_format(uint16_t product_id)
{
    switch (product_id) {
    case 0: 	case 1: 	case 2:		case 3:  case 4:  case 5:  case 6:  case 7:
    case 351: 	case 352: 	case 353:
    case 402: 	case 405:
        return "Text";

    case 8: 	case 9: 	case 10: 	case 11: case 12: case 13: case 14: case 15:
        return "Text/Graphic";
       
    case 20: 	case 21: 	case 22: 	case 23: case 24: case 25: case 26: case 27:
    case 411:	case 413:
        return "Text (DLAC)";

    case 51: 	case 52:	case 53: 	case 54: case 55: case 56: case 57: case 58:
    case 59: 	case 60: 	case 61: 	case 62: case 63: case 64: case 70: case 71:
    case 81: 	case 82: 	case 83: 	case 84:
    case 90:    case 91:
    case 101: 	case 102: 	case 103:	case 104:
    case 151:
    case 201: 	case 202:
    case 254:
    case 401:
    case 403:
    case 404:
        return "Graphic";
    case 412:
        return "Graphic (DLAC)";
    case 600: case 2004:
        return "Proprietary";
    case 2000: case 2001: case 2002: case 2003: case 2005: 
        return "Developmental";
    default:
        return "Unknown";
    }
}

static void uat_display_fisb_frame(const struct fisb_apdu *apdu,FILE *to)
{
	int recf;

	fprintf(to,"\nFIS-B: "
			" Flags:             %s%s%s%s "
			" Prod:  %u (%s) - %s\n",
			apdu->a_flag ? "A" : "",
            apdu->g_flag ? "G" : "",
            apdu->p_flag ? "P" : "",
            apdu->s_flag ? "S" : "",
            apdu->product_id,
            get_fisb_product_name(apdu->product_id),
            get_fisb_product_format(apdu->product_id));

    fprintf(to," PTime:");

    if (apdu->monthday_valid)
    	fprintf(to,"%u/%u ",apdu->month,apdu->day);

    fprintf(to,"%02u:%02u",apdu->hours,apdu->minutes);
    if (apdu->seconds_valid)
    	fprintf(to,":%02u",apdu->seconds);

    fprintf(to,"\n");

    switch (apdu->product_id) {
    case 8:             							// ** NOTAM **************
    	if ( apdu->s_flag)
    		recf = apdu->data[9] >> 4;
    	else
    		recf = apdu->data[0] >> 4;

    	fprintf(filenotam," Record Format   : %d\n",recf);

    	switch (recf){
    	case 8:  									//graphic
			fprintf(filenotam," Report Type     : NOTAM\n");
        	get_graphic(apdu,filenotam,to);
        	break;
    	case 2:                                   	// text
    		if (apdu->s_flag)          				// segmented text
    			get_seg_text(apdu,filenotam,to);
    		else  									// text
    			get_text(apdu,filenotam,to);
    		break;
    	default:
    		fprintf(to," Record Format   : %d\n",recf);
    		display_generic_data(apdu->data,apdu->length,to);
        	break;
    	}
    break;

    case 11:            							// ** AIRMET **************
    	recf = apdu->data[0] >> 4;
    	fprintf(fileairmet," Record Format   : %d \n",recf);

    	switch (recf){
    	case 8:  									//graphic
			fprintf(fileairmet," Report Type     : AIRMET\n");
			get_graphic(apdu,fileairmet,to);
			break;
    	case 2:										// text
        	get_text(apdu,fileairmet,to);
        	break;
    	default:
        	fprintf(to," Record Format   : %d \n",recf);
    		display_generic_data(apdu->data,apdu->length,to);
        	break;
    	}
    break;

    case 12:            							// ** SIGMET **************
    	recf = apdu->data[0] >> 4;
    	fprintf(filesigmet," Record Format   : %d \n",recf);

    	switch (recf) {
    	case 8: 									//graphic
    		fprintf(filesigmet," Report Type     : SIGMET\n");
    		get_graphic(apdu,filesigmet,to);
    		break;
    	case 2:  			   						// text
        	get_text(apdu,filesigmet,to);
        	break;
    	default:
        	fprintf(to," Record Format   : %d \n",recf );
        	display_generic_data(apdu->data,apdu->length,to);
        	break;
    	}
    break;

    case 13:            							// ** SUA **************
    	recf = apdu->data[0] >> 4;
    	fprintf(filesua," Record Format   : %d \n",recf);

    	switch (recf) {
    	case 2: 									//text
    		get_text(apdu,filesua,to);
    		break;
    	default:
    		fprintf(to," Record Format   : %d \n",recf);
    		display_generic_data(apdu->data,apdu->length,to);
    		break;
    	}
    break;

    case 14:            							// ** G-AIRMET **************
    	recf = apdu->data[0] >> 4;
    	fprintf(filegairmet," Record Format   : %d \n",recf);

    	switch (recf) {
    	case 8: 									//graphic
    		fprintf(to," Report Type     : G-AIRMET\n");
    		fprintf(filegairmet," Report Type     : G-AIRMET\n");
    		get_graphic(apdu,filegairmet,to);
    		break;
    	default:
    		fprintf(to," Record Format   : %d \n",recf);
    		display_generic_data(apdu->data,apdu->length,to);
    		break;
    	}
    break;

    case 15:            							// ** CWA **************
       	recf = apdu->data[0] >> 4;
       	fprintf(filesigmet," Record Format   : %d \n",recf);

       	switch (recf) {
       	case 8: 									//graphic
       		fprintf(filesigmet," Report Type     : CWA\n");
       		get_graphic(apdu,filecwa,to);
       		break;
       	case 2:     								// text
       		get_text(apdu,filecwa,to);
       		break;
       	default:
           	fprintf(to," Record Format   : %d \n",recf );
       		display_generic_data(apdu->data,apdu->length,to);
       		break;
       	}
    break;

    case 63: case 64:								// ** NEXRAD **************
    	graphic_nexrad(apdu,to);
    	break;

    case 70: case 71:								// ** Icing Low/High **************
    	graphic_nexrad(apdu,to);
    	break;

    case 84:  										// ** Cloud Tops **************
    	graphic_nexrad(apdu,to);
    	break;

    case 90: case 91:   							// ** Turbulence Low/high **************
    	graphic_nexrad(apdu,to);
        break;

    case 103:  										// ** Lightning  **************
    	graphic_nexrad(apdu,to);
        break;

    case 413:  										// ** Generic text,DLAC *****************
    {
    	int rec_offset = 0;
    	const char *text = decode_dlac(apdu->data,apdu->length,rec_offset);
    	const char *report = text;

    	while (report) {
    		char report_buf[1024];
    		const char *next_report;
    		char mtype[9]; char taftype[9];
    		char *p,*r;
    		char observation[900];
    		char gstn[5];
    		char *pirep_copy;

    		Decoded_METAR MetarStruct,*Mptr = &MetarStruct;

    		next_report = strchr(report,'\x1e'); // RS
    		if (!next_report)
    			next_report = strchr(report,'\x03'); // ETX
    		if (next_report) {
    			memcpy(report_buf,report,next_report - report);
    			report_buf[next_report - report] = 0;
    			report = next_report + 1;
    		}
    		else {
    			strcpy(report_buf,report);
    			report = NULL;
    		}
    		if (!report_buf[0])
    			continue;

    		r = report_buf;
    		strncpy(taftype,report_buf,7);
    		if (strcmp(taftype,"TAF COR") == 0){
    			report_buf[3]='.';
    		}
    		p = strchr(r,' ');    // *** RType ***
    		if (p) {
    			*p = 0;
    			strcpy(observation,r);
    			strncpy(mtype,r,8);
    			fprintf(to," RType: %s\n",mtype);
    			r = p+1;
    		}
    		p = strchr(r,' ');   // *** RLoc ***
    		if (p) {
    			*p = 0;
    			strcat(observation," ");
    			strcat(observation,r);
    			if (strcmp(mtype,"PIREP") == 0) {
    			}
    			else {
    				if( strcmp(mtype,"WINDS") == 0) {
    					strcpy(gstn,"K");
    					strcat(gstn,r);
    				}
    				else if (strcmp(mtype,"METAR") == 0 || strcmp(mtype,"SPECI") == 0   ) {
    					strncpy(gstn,r,5); }
    				else if (strcmp(mtype,"TAF") == 0 || strcmp(mtype,"TAF.AMD") == 0
    						|| strcmp(mtype,"TAF.COR") == 0 ) {
    					strncpy(gstn,r,5); }

    				get_gs_name(gstn);

    				time_t current_time = time(NULL);
    				struct tm *tm = localtime(&current_time);
    				fprintf(filemetar,"Time                 : %s",asctime(tm));
    				fprintf(filemetar,"WX Station           : %s - %s\n",gstn,gs_ret);
    				}
    			r = p+1;
    		}
    		p = strchr(r,' ');   //  *** RTime ***
    		if (p) {
    			*p = 0;
    			strcat(observation," ");
    			strcat(observation,r);
    			r = p+1;
    		}
    		if (strcmp(mtype,"TAF") == 0 || strcmp(mtype,"TAF.AMD" ) == 0 || strcmp(mtype,"TAF.COR") == 0){
    			fprintf(filemetar," Report Name         : %s\n",mtype);
    			fprintf(filemetar," Data:\n%s\n",r);    // *** Text ***
    		}
    		if (strcmp(mtype,"WINDS") == 0){
    			char *tok1;  char *tok2; char *tok3; char *tok4;
    			char winds[91];
    			char *q; char *u;

    			strncpy(winds,r,90);
    			winds[90]= '\0';
    			q=winds;
      			tok1 = strsep(&q,"\0");

    			fprintf(filemetar," Report Name         : %s\n",mtype);
    			fprintf(filemetar," Data:\n");

    			while ( (tok2 = strsep(&tok1," ")) != NULL ){
    				if (strcmp(tok2,"") != 0){
     					fprintf(filemetar,"%-10s",tok2);
    				}
    			}
    			fprintf(filemetar,"\n          ");

    			u = strchr(r,'\n');
    			u =u + 2;
    			tok3 = strsep(&u,"\0");

    			while ( (tok4 = strsep(&tok3," ")) != NULL ){
    				if (strcmp(tok4,"") != 0){
    					fprintf(filemetar,"%-10s",tok4);
    				}
    			}
   			fprintf(filemetar,"\n");
    		}
    		strcat(observation," ");
    		strcat(observation,r);

    		if (strcmp(mtype,"PIREP") == 0){
    			pirep_copy = (char *)malloc(strlen(r) + 1);
    			strcpy(pirep_copy,r);
    			get_pirep(pirep_copy,to);
    		}
    		if (strcmp(mtype,"METAR") == 0 || strcmp(mtype,"SPECI") == 0  ) {
    			if( decode_metar(observation,Mptr) != 0 ) {
    				fprintf(to,"Error METAR Decode\n"); }
    			else {
    				print_decoded_metar( Mptr);
    				if (metar_count == 0) {
    					fprintf(filemetarjson,"{\"type\": \"FeatureCollection\",\n");
    					fprintf(filemetarjson,"\"features\": [ \n");
    				}

    				float fahr;
    				float dewp;

    				if (MetarStruct.temp > 1000)
    					fahr = 999;
    				else
    					fahr = (MetarStruct.temp * 9/5 ) +32;
    				if (MetarStruct.dew_pt_temp > 1000)
    					dewp = 999;
    				else
    					dewp = (MetarStruct.dew_pt_temp * 9/5 ) +32;
    				if (MetarStruct.winData.windSpeed> 1000)
    					MetarStruct.winData.windSpeed=999;
    				if (MetarStruct.winData.windDir> 1000)
    					MetarStruct.winData.windDir=999;
    				if (MetarStruct.inches_altstng> 1000)
    					MetarStruct.inches_altstng=999;
    				if (metar_count == 0) {
    					fprintf(filemetarjson,"{\"type\": \"Feature\",\"properties\": { \"Stn\": \"%s\","
    							"\"ObDate\": \"%d %d:%d\","
    							"\"Temp\": \"%.2f\","
    							"\"WindSp\": \"%d\","
    							"\"WindDir\": \"%d\","
    							"\"Alt\": \"%3.2f\","
    							"\"Vsby\": \"%3.2f\","
    							"\"Loc\": \"%s\","
    							"\"DewP\": \"%.2f\""
    							"},\n",
								gstn,MetarStruct.ob_date,MetarStruct.ob_hour,MetarStruct.ob_minute,
								fahr,MetarStruct.winData.windSpeed,MetarStruct.winData.windDir,
								MetarStruct.inches_altstng,	MetarStruct.prevail_vsbySM,gs_ret,dewp);

    					fprintf(filemetarjson,"\"geometry\": { \"type\": \"Point\",\"coordinates\": [ %s,%s ] }}\n",
    							gs_ret_lng,gs_ret_lat);
    					fprintf(filemetarjson,"]}");
    					}
    				else {
    					fseek(filemetarjson,-3,SEEK_CUR);
    					fprintf(filemetarjson,",{\"type\": \"Feature\",\"properties\": { \"Stn\": \"%s\","
    							"\"ObDate\": \"%d %d:%d\","
    							"\"Temp\": \"%.2f\","
    							"\"WindSp\": \"%d\","
    							"\"WindDir\": \"%d\","
    							"\"Alt\": \"%3.2f\","
    							"\"Vsby\": \"%3.2f\","
    							"\"Loc\": \"%s\","
    							"\"DewP\": \"%.2f\""
    							"},\n",
								gstn,MetarStruct.ob_date,MetarStruct.ob_hour,MetarStruct.ob_minute,
								fahr,MetarStruct.winData.windSpeed,	MetarStruct.winData.windDir,
								MetarStruct.inches_altstng,MetarStruct.prevail_vsbySM,gs_ret,dewp);

    					fprintf(filemetarjson,"\"geometry\": { \"type\": \"Point\",\"coordinates\": [ %s,%s ] }}\n",
    							gs_ret_lng,gs_ret_lat);
    					fprintf(filemetarjson,"]}");
    				}
    				fflush(filemetarjson);
    				metar_count ++;
    			}
    		}
    		memset(&MetarStruct,0,sizeof(MetarStruct));
    		fflush(filemetar);
    	}
    }
    break;

    default:
    	display_generic_data(apdu->data,apdu->length,to);
    break;
    }                
}            

static void uat_display_uplink_info_frame(const struct uat_uplink_info_frame *frame,FILE *to)
{
	int tfr;
	int lidflag;
	int prod_range;
	int num_crl;
	uint16_t prodt;
	uint16_t repid = 0;

	fprintf(to,"\nINFORMATION FRAME:\n Type:  %u (%s)",
			frame->type,info_frame_type_names[frame->type]);

    if (frame->length > 0) {
    	if (frame->is_fisb)
    		uat_display_fisb_frame(&frame->fisb,to);
    	else {
    		int rec_offset=frame->length;
    		if (frame->type == 15) {
    			fprintf(to," ICAO List: \n");

    			int i = 1; int j =0	;
    			while (i < rec_offset ) {
    				if (j % 10 == 0)
    					fprintf(to,"\n");
    				j++;
    				fprintf(to,"%02x%02x%02x ",frame->data[i],frame->data[i+1],frame->data[i+2]);
    				i = i +4;
    			}
    			fprintf(to,"\n");
    		}
    		else if (frame->type == 14){      // report list
    			prodt = frame->data[0] <<3 | frame->data[1] >>5;
        		tfr = (frame->data[1] >> 4) & 1;
        		lidflag = (frame->data[1] >> 8) & 1;
        		prod_range = frame->data[2] * 5;
        		num_crl = frame->data[3];

        		fprintf(to,"\n Current Report List\n");
            	fprintf(to," Product: %d -  %s\n",prodt,get_fisb_product_name(prodt));
            	fprintf(to," TFR: %d LID Flag: %d",tfr,lidflag);
    	      	fprintf(to," Number of reports: %d  Range(nm): %d\n",num_crl,prod_range);

    	      	int j = 4;
    	      	int q = 0;
    	      	for (int i = 0; i < num_crl; ++i) {
    	      		if (q % 4 == 0)
    	      			fprintf(to,"\n");
    	      		q++;
    	      		repid = (frame->data[j+1] & ((1<<6)-1)) << 8 |  frame->data[j+2];

    	      		fprintf(to,"Rpt ID: %d  ",repid);
    	      		j = j+3;
    	      	}
    	      	fprintf(to,"\n");
    		}
    		else
    			display_generic_data(frame->data,frame->length,to);
    	}
    }
}

void uat_display_uplink_mdb(const struct uat_uplink_mdb *mdb,FILE *to)
{
	fprintf(to,"UPLINK: ");
	fprintf(to," Site: %u  ",mdb->tisb_site_id);
    fprintf(to," Lat: %+.4f%s Lon: %+.4f%s ",
    		mdb->lat,mdb->position_valid ? "" : " ",
    		mdb->lon,mdb->position_valid ? "" : " ");

    time_t current_time = time(NULL);
 	char * tm=ctime(&current_time);
    tm[strlen(tm)-6] = '\0';
    fprintf(to," Time: %s",tm);

    if (mdb->app_data_valid) {
    	unsigned i;
    	for (i = 0; i < mdb->num_info_frames; ++i)
    		uat_display_uplink_info_frame(&mdb->info_frames[i],to);
    }
}

static void get_graphic(const struct fisb_apdu  *apdu, FILE *fnm,FILE *to)
{
	int rec_offset = 11;
	int datoff = 6;
	int product_ver;
	int rec_count;
	int rec_ref;
	int overlay_rec_id = 0;
	int obj_label = 0;
	int obj_label_flag = 0;

	char gstn[5];
	char ob_type_text[35];
	char ob_ele_text[35];
	char ob_status_text[45];
	char geo_overlay_text[45];
	char obj_par_name_text[45];

	const char * obj_labelt;
	const char * location_id;

	uint16_t rec_len = 0;
	uint16_t rep_num = 0;
	uint16_t report_year = 0;

	uint8_t obj_type;
	uint8_t obj_element;
	uint8_t obj_status;
	uint8_t qualifier_flag;
	uint8_t param_flag;
	uint8_t rec_app_opt;
	uint8_t date_time_format;
	uint8_t element_flag;

	int geo_overlay_opt;
	int overlay_op;
	int overlay_vert_cnt;
	int d1;	int d2;	int d3;	int d4;

	uint32_t lat_raw;
	uint32_t lng_raw;
	uint32_t alt_raw;
	int alt;
	float lat;    float lat_save = 0.0;
	float lng;
	float fct_f =0.000687;  // float_t fct_t =0.001373;    raw_lon * 360.0 / 16777216.0;
//	float fct_f = 16777216.0;

	uint32_t object_qualifier;
	int obj_param_type = 0;
	uint16_t ob_par_val;

	char qual_text[200];
	char *start_date;
	char *stop_date;

	char *sql;
	char *zErrMsg = 0;
	int recf;
	FILE * file3dpoly;

	recf = apdu->data[0] >> 4;
	product_ver = ((apdu->data[0]) & 0x0F);
	rec_count = ((apdu->data[1]) & 0xF0) >> 4;
	rec_ref = ((apdu->data[5]));

	fprintf(fnm," Product Version : %d ",product_ver);
	fprintf(fnm,"           Record Count     : %d \n",rec_count);
	fprintf(fnm," Record Reference: %d \n",rec_ref);

	if (rec_ref != 255){
		location_id = decode_dlac(apdu->data,3,2);
		rec_offset = 2;
		const char *text = decode_dlac(apdu->data,5,rec_offset);
		strncpy(gstn,text,5);
		get_gs_name(gstn);

		fprintf(fnm," Location ID     : %s\n",location_id);
		fprintf(fnm," RLoc            : %s - %s\n",gstn,gs_ret);
	}
	else
		strcpy(gstn,"    ");


	rep_num = (((apdu->data[datoff + 1]) & 0x3F) << 8) | (apdu->data[datoff + 2]); 	//7 8
	rec_len = ((apdu->data[datoff + 0]) << 2) | (((apdu->data[datoff + 1]) & 0xC0) >> 6);    	// 6 7
	report_year = ((apdu->data[datoff + 3]) & 0xFE) >> 1;                            // 9
	overlay_rec_id = (((apdu->data[datoff + 4]) & 0x1E) >> 1) + 1; // Document instructs to add 1.
	obj_label_flag = (apdu->data[datoff + 4] & 0x01);                             				//10

	fprintf(fnm," Report Number   : %6d  ",rep_num);
	fprintf(fnm,"     Record Length    : %03d ",rec_len);
	fprintf(fnm,"     Report Year        : 20%02d\n ",report_year);
	fprintf(fnm,"Ovrlay RcID     : %d",overlay_rec_id);                       					//10
	fprintf(fnm,"            Object Label Flag: %d \n",obj_label_flag);

	obj_labelt ="  ";
	if (obj_label_flag == 0) { // Numeric index.
		obj_label = ((apdu->data[datoff + 5]) << 8) | (apdu->data[datoff +6]);   				// 11 12
		fprintf(fnm," Ob Lbl Num      : %d    ",obj_label);
		datoff = datoff +7;	}                   												//datoff=13
	else {
		obj_labelt = decode_dlac(apdu->data,5,2);
		fprintf(fnm," Ob Lbl Alph     : %s ",obj_labelt);
		datoff = datoff + 14;
	}

	element_flag = ((apdu->data[datoff + 0]) & 0x80) >> 7;                 			//13
	obj_element = (apdu->data[datoff + 0]) & 0x1F;                            		//13
	obj_status = (apdu->data[datoff +1]) & 0x0F;                   					//14
	obj_type = (apdu->data[datoff +1] & 0xF0) >> 4;                  				//14
	qualifier_flag = ((apdu->data[datoff + 0]) & 0x40) >> 6;           				//13
	param_flag = ((apdu->data[datoff + 0]) & 0x20) >> 5;                  			//13

	fprintf(fnm,"        Element Flag     : %d  ",element_flag);
	fprintf(fnm,"      Object Element     : %d\n",obj_element);
	fprintf(fnm," Object Status   : %d  ",obj_status);
	fprintf(fnm,"         Object Type      : %d \n",obj_type);
	fprintf(fnm," Qualifier Flag  : %d  ",qualifier_flag);
	fprintf(fnm,"          Parameter Flag   : %d \n",param_flag);
	 strcpy(qual_text," ");
	if (qualifier_flag == 0){
		datoff = datoff + 2;     													// 13 > datoff=15
	}
	else {
		object_qualifier = ((apdu->data[datoff + 2]) << 16) | ((apdu->data[datoff + 3]) << 8) | (apdu->data[datoff + 4]);

        strcpy(qual_text,"Qualifier: ");
		if (apdu->product_id == 14) {
			if (apdu->data[datoff + 2] & (1 << 7)){                                  //15
				fprintf(fnm," Qualifier       : Unspecified\n");
				strcat(qual_text," Unspecified, ");
			}

			if (apdu->data[datoff + 3] & (1 << 0)) {                                  //16
				fprintf(fnm," Qualifier       : Ash\n");
				strcat(qual_text," Ash, ");
			}

			if (apdu->data[datoff + 4] & (1 << 0)) {                                 //17
				fprintf(fnm," Qualifier       : Precipitation\n");
				strcat(qual_text," Precipitation, ");
			}

			if (apdu->data[datoff + 4] & (1 << 1))  {
				fprintf(fnm," Qualifier       : Mist\n");
				strcat(qual_text," Mist, ");
			}
			if (apdu->data[datoff + 4] & (1 << 2)){
				fprintf(fnm," Qualifier       : Fog\n");
				strcat(qual_text," Fog, ");
			}
			if (apdu->data[datoff + 4] & (1 << 3)){
				fprintf(fnm," Qualifier       : Haze\n");
				strcat(qual_text," Haze, ");
			}
			if (apdu->data[datoff + 4] & (1 << 4)) {
				fprintf(fnm," Qualifier       : Smoke\n");
				strcat(qual_text," Smoke, ");
			}
			if (apdu->data[datoff + 4] & (1 << 5)) {
				fprintf(fnm," Qualifier       : Blowing Snow\n");
				strcat(qual_text," Blowing Snow, ");
			}
			if (apdu->data[datoff + 4] & (1 << 6)) {
				fprintf(fnm," Qualifier       : Clouds\n");
				strcat(qual_text," Clouds, ");
			}
			if (apdu->data[datoff + 4] & (1 << 7)) {
				fprintf(fnm," Qualifier      : Dust\n");
				strcat(qual_text," Dust, ");
			}
		}
		obj_param_type = apdu->data[18] >> 3;              					        //18
		ob_par_val = (apdu->data[18] & 0x7)<< 8 | apdu->data[19];					//19

		fprintf(fnm,"Obj Qualfr: %d  Obj Prm Tp: %d  Obj Par Val: %d\n",
				object_qualifier,obj_param_type,ob_par_val);

		datoff = datoff+7;                 //13 datogg =20
	}

	geo_overlay_opt = (apdu->data[datoff + 0]) & 0x0F;                              //13
	overlay_op = ((apdu->data[datoff +1]) & 0xC0) >> 6;
	overlay_vert_cnt = ((apdu->data[datoff +1]) & 0x3F) + 1; // Document instructs to add 1)
	rec_app_opt = ((apdu->data[datoff + 0]) & 0xC0) >> 6;
	date_time_format = ((apdu->data[datoff + 0]) & 0x30) >> 4;

	fprintf(fnm," Geo Overlay Opts: %02d  ",geo_overlay_opt);
	fprintf(fnm,"         Overlay Operator : %d  ",overlay_op);
	fprintf(fnm,"      Overlay Vertices # : %d \n",overlay_vert_cnt);
	fprintf(fnm," Rec App Option  : %d  ",rec_app_opt);
	fprintf(fnm,"          Rec App Date     : %d \n",date_time_format );

	switch (rec_app_opt) {
	case 0:  									// No times given. UFN.  (record_data[2:],date_time_format)
		fprintf(fnm,"No Dates Given\n");
		asprintf(&start_date,"0");
		asprintf(&stop_date,"0");
		datoff = datoff +2;
		break;

	case 1:  									// Start time only. WEF.
		d1 = apdu->data[datoff + 2];
		d2 = apdu->data[datoff + 3];
		d3 = apdu->data[datoff + 4];
		d4 = apdu->data[datoff + 5];
		fprintf(fnm," Only Start Date : %02d/%02d %02d:%02d \n",d1,d2,d3,d4);
		asprintf(&start_date,"%02d/%02d %02d:%02d",d1,d2,d3,d4);
		asprintf(&stop_date,"0");
		datoff = datoff + 6;
		break;

	case 2: 									// End time only. TIL.
		d1 = apdu->data[datoff + 2];
		d2 = apdu->data[datoff + 3];
		d3 = apdu->data[datoff + 4];
		d4 = apdu->data[datoff + 5];
		fprintf(fnm," Only End Date: %02d/%02d %02d:%02d \n",d1,d2,d3,d4);
		asprintf(&start_date,"0");
		asprintf(&stop_date,"%02d/%02d %02d:%02d",d1,d2,d3,d4);
		datoff = datoff + 6;
		break;

	case 3: // Both start and end times. WEF.
		d1 = apdu->data[datoff + 2];
		d2 = apdu->data[datoff + 3];
		d3 = apdu->data[datoff + 4];
		d4 = apdu->data[datoff + 5];
		fprintf(fnm," Start Date      : %02d/%02d %02d:%02d  ",d1,d2,d3,d4);
		asprintf(&start_date,"%02d/%02d %02d:%02d",d1,d2,d3,d4);
		d1 = apdu->data[datoff + 6];
		d2 = apdu->data[datoff + 7];
		d3 = apdu->data[datoff + 8];
		d4 = apdu->data[datoff + 9];
		fprintf(fnm,"End Date         : %02d/%02d %02d:%02d \n",d1,d2,d3,d4);
		asprintf(&stop_date,"%02d/%02d %02d:%02d",d1,d2,d3,d4);
		datoff = datoff + 10;
		break;
	}

	fprintf(fnm,"\n");

	strcpy(ob_type_text,"Unknown Object");
	strcpy(ob_ele_text,"Unknown Element");
	strcpy(ob_status_text,"Unknown Status");
	strcpy(ob_status_text,object_status_text[obj_status]);
	strcpy(geo_overlay_text,overlay_options_text[geo_overlay_opt]);

	if (qualifier_flag == 1)
		strcpy(obj_par_name_text,obj_param_type_names[obj_param_type]);
	else
		strcpy(obj_par_name_text,"n/a");

	if (obj_label_flag == 0)
		strcpy(ob_type_text,object_types_text[obj_type]);
	if (element_flag == 1 && obj_type == 14)
		strcpy(ob_ele_text,airspace_element_names[obj_element]);
	if (element_flag == 1 && obj_type == 14 && apdu->product_id == 14)
		strcpy(ob_ele_text,gairspace_element_names[obj_element]);

	time_t current_time = time(NULL);
	struct tm *tm = localtime(&current_time);
	fprintf(fnm,"Time            : %s",asctime(tm));

	asprintf(&sql,"INSERT INTO graphic_reports (prod_id,rec_format,stn_call,prod_ver,rec_count,rec_ref,"
			"rep_number,rec_length,rep_year,ovrly_recid,obj_lbl_flag,obj_lbl_number,obj_lbl_alpha,"
			"element_flag,obj_element,obj_status,obj_type,qual_flag,param_flag,rec_app_option,"
			"app_opt_fmt,start_date,stop_date,geo_ovrly_opt,ovrly_operator,ovrly_vertices,qual_text,rep_rec_time)"
			"VALUES(%d,%d,'%s',%d,%d,%d,%d,%d,%d,%d,%d,%d,'%s',%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s',%d,%d,%d,'%s','%s')",
			apdu->product_id,recf,gstn,product_ver,rec_count,rec_ref,rep_num,rec_len,report_year,overlay_rec_id,obj_label_flag,obj_label,obj_labelt,element_flag,obj_element,obj_status,obj_type,qualifier_flag,param_flag,rec_app_opt,date_time_format,start_date,stop_date,geo_overlay_opt,overlay_op,overlay_vert_cnt,qual_text,asctime(tm));

	rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}

	switch (geo_overlay_opt) {
	case 3:  case 4: 								// Extended Range 3D Polygon
		alt_raw = (((apdu->data[datoff +4]) & 0x03) << 8) | (apdu->data[datoff +5]);
		alt = alt_raw * 100;

		switch(apdu->product_id ){
		case 11:
			file3dpoly = fileairmetjson;
			airmet_count = ex3dpoly(fileairmetjson,airmet_count,rep_num,alt,ob_ele_text);
			break;
		case 12:
			file3dpoly = filesigmetjson;
			sigmet_count = ex3dpoly(filesigmetjson,sigmet_count,rep_num,alt,ob_ele_text);
			break;
		case 14:
			file3dpoly = filegairmetjson;
			gairmet_count = ex3dpoly(filegairmetjson,gairmet_count,rep_num,alt,ob_ele_text);
			break;
		case 15:
			file3dpoly = filecwajson;
			cwa_count = ex3dpoly(filecwajson,cwa_count,rep_num,alt,ob_ele_text);
			break;

		case 18:        file3dpoly = filenotamjson;
			if (notam_count == 0) {
				fprintf(file3dpoly,"{\"type\": \"FeatureCollection\",\n");
				fprintf(file3dpoly,"\"features\": [ \n");
				fprintf(file3dpoly,"{\"type\": \"Feature\",\"properties\": { \"RepNum\": \"%d\",\"Alt\": \"%d\",\"Ob\": \"%s\"},\n",rep_num,alt,ob_ele_text);
				fprintf(file3dpoly,"\"geometry\": { \"type\": \"Polygon\",\"coordinates\": [[\n");
			}
			else {
				fseek(file3dpoly,-3,SEEK_CUR);
				fprintf(file3dpoly,",{\"type\": \"Feature\",\"properties\": { \"RepNum\": \"%d\",\"Alt\": \"%d\",\"Ob\": \"%s\"},\n",rep_num,alt,ob_ele_text);
				fprintf(file3dpoly,"\"geometry\": { \"type\": \"Polygon\",\"coordinates\": [[\n");
			}
			notam_count++;
			break;
			}

		for (int i = 0; i < overlay_vert_cnt; i++) {
			lng_raw = ((apdu->data[datoff + i]) << 11) | ((apdu->data[datoff +i+1]) << 3) | ((apdu->data[datoff +i+2]) & 0xE0 >> 5);
			lat_raw = (((apdu->data[datoff +i+2]) & 0x1F) << 14) | ((apdu->data[datoff +i+3]) << 6) | (((apdu->data[datoff +i+4]) & 0xFC) >> 2);
			alt_raw = (((apdu->data[datoff +i+4]) & 0x03) << 8) | (apdu->data[datoff +i+5]);

			datoff = datoff + 5;

			lat = fct_f * lat_raw;
			lng = fct_f * lng_raw;
			if (lat > 90.0)
				lat = lat - 180.0;
			if (lng > 180.0)
				lng = lng - 360.0;
			alt = alt_raw * 100;

			if (lat == lat_save){                 // Ignore 2nd altitude data to fix map
				fseek(file3dpoly,-2,SEEK_CUR);
				i = overlay_vert_cnt;
				continue;
			}
			lat_save = lat;

			if (i == (overlay_vert_cnt-1))
				fprintf(file3dpoly,"[ %f, %f ]\n",lng,lat);
			else
				fprintf(file3dpoly,"[ %f, %f ],\n",lng,lat);

			fprintf(fnm,"      Coordinates: %11f,%11f    Alt: %d\n",lat,lng,alt);
		}

		fprintf(file3dpoly,"]]\n");
		fprintf(file3dpoly,"}}\n");
		fprintf(file3dpoly,"]}\n");
		fflush(file3dpoly);
		break;

	case 7: case 8: 								// Extended Range Circular Prism (7 = MSL,8 = AGL)
		if (rec_len < 14) {
			fprintf(fnm,"Data too short. Should be 14 bytes; %d seen.\n",rec_len);
		}
		else {
			uint32_t lng_bot_raw;
			uint32_t lat_bot_raw;
			uint32_t lng_top_raw;
			uint32_t lat_top_raw;

			uint32_t alt_bot_raw;
			uint32_t alt_top_raw;
			uint32_t r_lng_raw;
			uint32_t r_lat_raw,alpha;
			uint32_t alt_bot;
			uint32_t alt_top;

			float lat_bot,lng_bot,lat_top,lng_top,r_lng,r_lat;

			lng_bot_raw = ((apdu->data[datoff +0]) << 10) | ((apdu->data[datoff +1]) << 2) | ((apdu->data[datoff +2]) & 0xC0 >> 6);
			lat_bot_raw = (((apdu->data[datoff+ 2]) & 0x3F) << 12) | ((apdu->data[datoff +3]) << 4) | (((apdu->data[datoff +4]) & 0xF0) >> 4);
			lng_top_raw = (((apdu->data[datoff +4]) & 0x0F) << 14) | ((apdu->data[datoff +5]) << 6) | (((apdu->data[datoff +6]) & 0xFC) >> 2);
			lat_top_raw = (((apdu->data[datoff +6]) & 0x03) << 16) | ((apdu->data[datoff +7]) << 8) | (apdu->data[datoff +8]);
			alt_bot_raw = ((apdu->data[datoff +9]) & 0xFE) >> 1;
			alt_top_raw = (((apdu->data[datoff +9]) & 0x01) << 6) | (((apdu->data[datoff +10]) & 0xFC) >> 2);
			r_lng_raw = (((apdu->data[datoff +10]) & 0x03) << 7) | (((apdu->data[datoff +11]) & 0xFE) >> 1);
			r_lat_raw = (((apdu->data[datoff +11]) & 0x01) << 8) | (apdu->data[datoff +12]);
			alpha = (apdu->data[datoff +13]);
			lng_bot_raw = (~lng_bot_raw & 0x1FFFF) +1;    // 2's compliment +1
			lat_bot_raw = (~lat_bot_raw & 0x1FFFF) +1;    // 2's compliment +1
			lat_bot = lat_bot_raw *  0.001373 ;
			lng_bot = (lng_bot_raw *  0.001373) * -1 ;

			if (lat_bot > 90.0)
				lat_bot = (lat_bot - 180.0) * -1 ;
			if (lng_bot > 180.0)
				lng_bot = lng_bot - 360.0;

			lng_top_raw = (~lng_top_raw & 0x1FFFF) +1;    // 2's compliment +1
			lat_top_raw = (~lat_top_raw & 0x1FFFF) +1;    // 2's compliment +1
			lat_top = lat_top_raw *  0.001373 ;
			lng_top = (lng_top_raw *  0.001373) * -1 ;

			if (lat_top > 90.0)
				lat_top = (lat_top - 180.0) * -1 ;
			if (lng_top > 180.0)
				lng_top = lng_top - 360.0;

			alt_bot = alt_bot_raw * 5;
			alt_top = alt_top_raw * 500;
			r_lng = r_lng_raw * 0.2;
			r_lat = r_lat_raw * 0.2;

			fprintf(fnm,"lat_bot, lng_bot = %f, %f\n", lat_bot, lng_bot);
			fprintf(fnm,"lat_top, lng_top = %f, %f\n", lat_top, lng_top);

			if (geo_overlay_opt == 8)
				fprintf(fnm,"alt_bot, alt_top = %d AGL, %d AGL  geo_opt:%d\n", alt_bot, alt_top,geo_overlay_opt);
			else
				fprintf(fnm,"alt_bot, alt_top = %d MSL, %d MSL  geo_opt:%d\n", alt_bot, alt_top,geo_overlay_opt);

			fprintf(fnm,"r_lng, r_lat = %f, %f\n", r_lng,r_lat);
			fprintf(fnm,"alpha=%d\n",alpha);
		}
		break;

	case 9: 									// Extended Range 3D Point (AGL)
		if (rec_len < 6) {
			fprintf(fnm, "Too short\n");
		}
		else {
			lng_raw = ((apdu->data[datoff +0]) << 11) | ((apdu->data[datoff +1]) << 3) | ((apdu->data[datoff +2]) & 0xE0 >> 5);
			lat_raw = (((apdu->data[datoff +2]) & 0x1F) << 14) | ((apdu->data[datoff + 3]) << 6) | (((apdu->data[datoff + 4]) & 0xFC) >> 2);
			alt_raw = (((apdu->data[datoff + 4]) & 0x03) << 8) | (apdu->data[datoff + 5]);
			lng_raw = (~lng_raw & 0x7FFFF) +1;    // 2's compliment +1
			lat_raw = (~lat_raw & 0x7FFFF) +1;    // 2's compliment +1
			lat = lat_raw *  0.0006866455078125 ;
			lng = (lng_raw *  0.0006866455078125) * -1 ;
			if (lat > 90.0)
				lat = 360.0 - lat;
			if (lng > 180.0)
				lng = lng - 360.0;
			alt = alt_raw * 100;

			if (apdu->product_id == 18) {
				notam_list[notam_count].notam_lat = lat;
				notam_list[notam_count].notam_lng = lng;
				strcpy(notam_list[notam_count].notam_stn,gstn);
				notam_list[notam_count].notam_repid =rep_num;
				if (notam_count == 0) {
					fprintf(filenotamjson,"{\"type\": \"FeatureCollection\",\n");
					fprintf(filenotamjson,"\"features\": [ \n");
					fprintf(filenotamjson,"{\"type\": \"Feature\",\"properties\": { \"Location\": \"%s\",\"Stuff\": \"%d\"},\n",gstn,rep_num);
					fprintf(filenotamjson,"\"geometry\": { \"type\": \"Point\",\"coordinates\": [ %f,%f ] }}\n",lng,lat);
					fprintf(filenotamjson,"]}");
				}
				else {
					fseek(filenotamjson,-3,SEEK_CUR);
					fprintf(filenotamjson,",{\"type\": \"Feature\",\"properties\": { \"Location\": \"%s\",\"Stuff\": \"%d\"},\n",gstn,rep_num);
					fprintf(filenotamjson,"\"geometry\": { \"type\": \"Point\",\"coordinates\": [ %f,%f ] }}\n",lng,lat);
					fprintf(filenotamjson,"]}");
				}

				fflush(filenotamjson);
				notam_count ++;
			}
			fprintf(fnm,"      Coordinates: %11f,%11f    Alt: %d\n",lat,lng,alt);
		}
		break;

	case 11: case 12:   							// Extended Range 3D Polyline
		                							// Don't write geojson for items with qualifier flag set.

		alt_raw = (((apdu->data[datoff +4]) & 0x03) << 8) | (apdu->data[datoff +5]);
		alt = alt_raw * 100;

		switch(apdu->product_id ){
		case 14:
			file3dpoly = filegairmetjson;
			if (qualifier_flag == 0){
				if (gairmet_count == 0) {
					fprintf(file3dpoly,"{\"type\": \"FeatureCollection\",\n");
					fprintf(file3dpoly,"\"features\": [ \n");
					fprintf(file3dpoly,"{\"type\": \"Feature\",\"properties\": { \"RepNum\": \"%d\",\"Alt\": \"%d\",\"Ob\": \"%s\"},\n",rep_num,alt,ob_ele_text);
					fprintf(file3dpoly,"\"geometry\": { \"type\": \"LineString\",\"coordinates\": [\n");
				}
				else {
					fseek(file3dpoly,-3,SEEK_CUR);
					fprintf(file3dpoly,",{\"type\": \"Feature\",\"properties\": { \"RepNum\": \"%d\",\"Alt\": \"%d\",\"Ob\": \"%s\"},\n",rep_num,alt,ob_ele_text);
					fprintf(file3dpoly,"\"geometry\": { \"type\": \"LineString\",\"coordinates\": [\n");
				}
				gairmet_count++;
			}
			break;
		}
		for (int i = 0; i < overlay_vert_cnt; i++) {
			if (rec_len < 6) {
				fprintf(fnm, "Too short\n");
			}
			else {
				lng_raw = ((apdu->data[datoff +i]) << 11) | ((apdu->data[datoff +i+1]) << 3) | ((apdu->data[datoff +i+2]) >> 5);
				lat_raw = (((apdu->data[datoff +i+2]) & 0x1F) << 14) | ((apdu->data[datoff +i+ 3]) << 6) | (((apdu->data[datoff +i+ 4]) & 0xFC) >> 2);
				alt_raw = (((apdu->data[datoff +i+ 4]) & 0x03) << 8) | (apdu->data[datoff +i+ 5]);

				datoff = datoff + 5;

				lng_raw = (~lng_raw & 0x7FFFF) +1;    // 2's compliment +1
				lat_raw = (~lat_raw & 0x7FFFF) +1;    // 2's compliment +1
				lat = lat_raw *  0.0006866455078125 ;
				lng = lng_raw * -0.0006866455078125 ;
				if (lat > 90.0)
					lat = 360.0 - lat;
				if (lng > 180.0)
					lng = lng - 360.0;
				alt = alt_raw * 100;

				if (qualifier_flag == 0){
					if (i == (overlay_vert_cnt-1))
						fprintf(file3dpoly,"[ %f,%f ]\n",lng,lat)	;
					else
						fprintf(file3dpoly,"[ %f,%f ],\n",lng,lat);
				}
				fprintf(fnm,"      Coordinates: %11f,%11f    Alt: %d\n",lat,lng,alt);
			}
		}
		if (qualifier_flag == 0){
			fprintf(file3dpoly,"]\n");
			fprintf(file3dpoly,"}}\n");
			fprintf(file3dpoly,"]}\n");
			fflush(file3dpoly);
		}
		break;
	}

	fprintf(fnm,"\n");
	fprintf(fnm," Object Type: %s  Object Element: %s\n",ob_type_text,ob_ele_text);
	fprintf(fnm," Object Status: %s Overlay type: %s\n",ob_status_text,geo_overlay_text);
	fprintf(fnm," obj_par_name_text: %s\n",obj_par_name_text);
	fprintf(fnm,"\n");
	fflush(fnm);

//	display_generic_data(apdu->data,apdu->length,to);
}

static void get_text(const struct fisb_apdu  *apdu, FILE *fnm,FILE *to){

	int rec_offset = 11;
	char gstn[5];
	char *sua_text;
	const char *text = decode_dlac(apdu->data,apdu->length,rec_offset);
	const char *report = text;

   	while (report) {
   		char report_buf[1024];
   		char *p,*r;
   		const char *next_report; uint16_t report_year;
   		uint16_t rep_num = 0;

   		next_report = strchr(report,'\x1e'); // RS
   		if (!next_report)
   			next_report = strchr(report,'\x03'); // ETX
   		if (next_report) {
   			memcpy(report_buf,report,next_report - report);
   			report_buf[next_report - report] = 0;
   			report = next_report + 1;
   		}
   		else {
   			strcpy(report_buf,report);
   			report = NULL;
   		}
   		if (!report_buf[0])
   			continue;

   		r = report_buf;
   		p = strchr(r,' ');
   		if (p) {
   			*p = 0;
   			fprintf(fnm," Report Type     : %s\n",r);
   			r = p+1;
   		}
   		if (apdu->product_id == 8)
   			p = strchr(r,'.');
   		else
   			p = strchr(r,' ');

   		if (apdu->product_id != 13){
   			if (p) {
   				*p = 0;
   				strncpy(gstn,r,5);
   				get_gs_name(gstn);
   				fprintf(fnm," RLoc            : %s - %s\n",gstn,gs_ret);
   				r = p + 1;
   			}
   		}
   		p = strchr(r,' ');   //  *** RTime ***
		if (p) {
			*p = 0;
			fprintf(fnm," RTime           : %s\n",r);
			r = p + 1;
		}

		rep_num = ((apdu->data[8]  << 6) | (apdu->data[9] & 0xFC) >> 2);
		fprintf(fnm," Report Number   : %6d  ",rep_num);
    	report_year = (((apdu->data[9]) & 0x03) << 5 | ((apdu->data[10])  & 0xF8) >> 3) ;
    	fprintf(fnm,"     Report Year       : 20%02d\n ",report_year);

   		time_t current_time = time(NULL);
   		struct tm *tm = localtime(&current_time);
   		fprintf(fnm,"Time            : %s",asctime(tm));

   		if (apdu->product_id == 13){
   			sua_text = (char *)malloc(strlen(r) + 1);
   			strcpy(sua_text,r);
   			get_sua_text(sua_text,to);
   		}

   		if (apdu->product_id == 8){
   			char *zErrMsg = 0;
   			char *sql;
   			int length = strlen(r);
   			if (r[length-1] == '\n')
   			    r[length-1] = '\0';

   			for(int i = 0; i <= strlen(r); i++)
   			{
   				if(r[i] == '\n')
   				{
   					r[i] = ' ';
   				}
   			}
   			if (notam_count == 0) {
   				fprintf(filenotamjson,"{\"type\": \"FeatureCollection\",\n");
   				fprintf(filenotamjson,"\"features\": [ \n");
   				fprintf(filenotamjson,"{\"type\": \"Feature\",\"properties\": { \"Data\": \"%s\",\"RepNum\": \"%d\",\"Stn\": \"%s\",\"Loc\": \"%s\"},\n",r,rep_num,gstn,gs_ret);
   				fprintf(filenotamjson,"\"geometry\": { \"type\": \"Point\",\"coordinates\": [ %s,%s ] }}\n",gs_ret_lng,gs_ret_lat);
   			}
   			else {
   				fseek(filenotamjson,-3,SEEK_CUR);
   				fprintf(filenotamjson,",{\"type\": \"Feature\",\"properties\": { \"Data\": \"%s\",\"RepNum\": \"%d\",\"Stn\": \"%s\",\"Loc\": \"%s\"},\n",r,rep_num,gstn,gs_ret);
   				fprintf(filenotamjson,"\"geometry\": { \"type\": \"Point\",\"coordinates\": [ %s,%s ] }}\n",gs_ret_lng,gs_ret_lat);
   			}
   			fprintf(filenotamjson,"]}\n");

   			notam_count++;

   			// add to database
  			asprintf(&sql,"INSERT INTO notam (station_name,station_loc,report_num) VALUES ('%s','%s',%d )",gstn,gs_ret,rep_num);
   			rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
   			if( rc != SQLITE_OK ){
   				fprintf(stderr, "SQL error: %s\n", zErrMsg);
   				sqlite3_free(zErrMsg);
   			}
   		}

   		fprintf(fnm," Data:\n%s\n",r);
		fflush(fnm);
   	}
}

static void get_seg_text(const struct fisb_apdu  *apdu, FILE *fnm,FILE *to)
{
	uint16_t prodid;
	uint16_t prodfillen;
	uint16_t apdunum;

	prodid = ((apdu->data[4] & 0x7) >> 1) | (apdu->data[5] >> 1);
	prodfillen = (apdu->data[5] & 0x1) | (apdu->data[6]);
	apdunum = ((apdu->data[7]) << 1 ) | (apdu->data[8] >> 7);

	int fg = 0;       								// Check if report part already stored
	for (int i = 0; i <= seg_count; ++i) {
		if ((prodid == seg_list[i].seg_prodid) &&
				(prodfillen == seg_list[i].seg_prolen) &&
				(apdunum == seg_list[i].seg_segnum)){
			++fg;
		}
	}
	if (fg == 0){              						// New report part - add record
		int offx = 0;
		seg_list[seg_count].seg_prodid = prodid;
		seg_list[seg_count].seg_prolen = prodfillen;
		seg_list[seg_count].seg_segnum = apdunum;

		if (apdunum == 1){            				// Contains report number and year
			offx = 20;
			seg_list[seg_count].seg_text_len = apdu->length-20;
			seg_list[seg_count].seg_rpt_num = ((apdu->data[17]  << 6) | (apdu->data[18] >> 2));   			//7 8
			seg_list[seg_count].seg_rpt_year = (((apdu->data[18]) & 0x03) << 5 | ((apdu->data[19])  & 0xF8) >> 3) ;
		}
		else {
			offx = 15;
			seg_list[seg_count].seg_text_len = apdu->length-15;
			seg_list[seg_count].seg_rpt_num = 0;
			seg_list[seg_count].seg_rpt_year = 0 ;
		}
		for ( int x = offx; x <= apdu->length; ++x) {
			int c = apdu->data[x];
			seg_list[seg_count].seg_data[x-offx] = c;
		}
		++seg_count;
	}
	fg = 0;     									// See if all parts of record are stored
	for (int i = 0; i <= seg_count; ++i) {
		for (int j = 1; j <= prodfillen; ++j) {
			if ((prodid == seg_list[i].seg_prodid) &&
					(prodfillen == seg_list[i].seg_prolen) &&
					(j == seg_list[i].seg_segnum)){
				++fg;
			}
		}
	}
	if (fg == prodfillen){           				// All parts have been stored
		uint8_t rep_all[2000];
		int char_cnt = 0;
		for (int i = 0; i <= seg_count; ++i) {      // 1st part
			if ((seg_list[i].seg_prodid == prodid) &&
					(seg_list[i].seg_prolen == prodfillen) && (seg_list[i].seg_segnum == 1)){
				for (int d = char_cnt; d  <= seg_list[i].seg_text_len; ++d) {   // 0 to 550
					rep_all[d] = seg_list[i].seg_data[d];
				}
				char_cnt = char_cnt + seg_list[i].seg_text_len;	  // 550

				fprintf(fnm," Report Type     : NOTAM - Segmented\n");
				fprintf(fnm," Num of Segments : %d\n",seg_list[i].seg_prolen);
				fprintf(fnm," Report Number   : %6d  ",seg_list[i].seg_rpt_num);
		    	fprintf(fnm,"     Report Year       : 20%02d\n ",seg_list[i].seg_rpt_year);
			}
		}
		for (int i = 0; i <= seg_count; ++i) {      // 2nd part
			if ((seg_list[i].seg_prodid == prodid) &&
					(seg_list[i].seg_prolen == prodfillen) && (seg_list[i].seg_segnum == 2)){
				for (int d = char_cnt; d  <= char_cnt + seg_list[i].seg_text_len; ++d) {
					rep_all[d] = seg_list[i].seg_data[(d-char_cnt)];
				}
				char_cnt = char_cnt + seg_list[i].seg_text_len;
			}
		}
		for (int i = 0; i <= seg_count; ++i) {      // 3rd part
			if ((seg_list[i].seg_prodid == prodid) &&
					(seg_list[i].seg_prolen == prodfillen) && (seg_list[i].seg_segnum == 3)){
				for (int d = char_cnt; d  <=  char_cnt + seg_list[i].seg_text_len; ++d) {
					rep_all[d] = seg_list[i].seg_data[(d-char_cnt)];
				}
				char_cnt = char_cnt + seg_list[i].seg_text_len;
			}
		}

		const char *seg_text_all = decode_dlac(rep_all,char_cnt,0);

		time_t current_time = time(NULL);
		struct tm *tm = localtime(&current_time);
		fprintf(fnm,"Time            : %s",asctime(tm));
		fprintf(fnm," Data:\n%s\n\n",seg_text_all);
	}

//	display_generic_data(apdu->data,apdu->length,to);
	fflush(fnm);
}
