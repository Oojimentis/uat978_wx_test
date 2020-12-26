// Part of dump978, a UAT decoder.
// Copyright 2015, Oliver Jowett <oliver@mutability.co.uk>
//

#ifndef UAT_DECODE_H
#define UAT_DECODE_H

#include <stdint.h>

#include <stdio.h>
#include <sqlite3.h>
#include "uat.h"

//
// Datatypes
//

	sqlite3 *db;
	int rc;

typedef enum { AQ_ADSB_ICAO=0, AQ_NATIONAL=1, AQ_TISB_ICAO=2, AQ_TISB_OTHER=3, AQ_VEHICLE=4,
	AQ_FIXED_BEACON=5, AQ_RESERVED_6=6, AQ_RESERVED_7=7 } address_qualifier_t;
typedef enum { ALT_INVALID=0, ALT_BARO, ALT_GEO } altitude_type_t;
typedef enum { AG_SUBSONIC=0, AG_SUPERSONIC=1, AG_GROUND=2, AG_RESERVED=3 } airground_state_t;
typedef enum { TT_INVALID=0, TT_TRACK, TT_MAG_HEADING, TT_TRUE_HEADING } track_type_t;
typedef enum { HT_INVALID=0, HT_MAGNETIC, HT_TRUE } heading_type_t;
typedef enum { CS_INVALID=0, CS_CALLSIGN, CS_SQUAWK } callsign_type_t;

// Segmentation
struct MySegStruct{
	uint16_t seg_prodid;
	uint16_t seg_prolen;
	uint16_t seg_segnum;
	int seg_text_len;
	uint16_t seg_rpt_num;
	int seg_rpt_year;
	uint8_t  seg_data[1000];
}  seg_list[50];

int seg_count;

struct NotamJsonStruct{
	float notam_lat;
	float notam_lng;
	char  notam_stn[6];
	int  notam_repid;
} notam_list[5000];

int notam_count;
int metar_count;
int airmet_count;
int gairmet_count;
int sigmet_count;
int cwa_count;


FILE * filemetar;   	// 413
FILE * filenotam;   	// 8
FILE * filesua;      	// 13
FILE * filesigmet;    	// 12
FILE * fileairmet;		// 11
FILE * filegairmet;		// 14
FILE * filepirep;		// PIREP
/*
FILE * filenexradc;		// NEXRAD Graphics output
FILE * fileicingl2;		// ICING LOW Graphics output
FILE * fileicingl4;		// ICING LOW Graphics output
FILE * fileicingl6;		// ICING LOW Graphics output
FILE * fileicingl8;		// ICING LOW Graphics output
FILE * fileicingl10;	// ICING LOW Graphics output
FILE * fileicingl12;	// ICING LOW Graphics output
FILE * fileicingl14;	// ICING LOW Graphics output
FILE * fileicingl16;	// ICING LOW Graphics output
FILE * fileicingh18;	// ICING HIGH Graphics output
FILE * fileicingh20;	// ICING HIGH Graphics output
FILE * fileicingh22;	// ICING HIGH Graphics output
FILE * fileicingh24;	// ICING HIGH Graphics output
FILE * filecloudt;		// CLOUD TOPS Graphics output
FILE * fileturbl2;		// TURBULENCE LOW Graphics output
FILE * fileturbl4;		// TURBULENCE LOW Graphics output
FILE * fileturbl6;		// TURBULENCE LOW Graphics output
FILE * fileturbl8;		// TURBULENCE LOW Graphics output
FILE * fileturbl10;		// TURBULENCE LOW Graphics output
FILE * fileturbl12;		// TURBULENCE LOW Graphics output
FILE * fileturbl14;		// TURBULENCE LOW Graphics output
FILE * fileturbl16;		// TURBULENCE LOW Graphics output
FILE * fileturbh18;		// TURBULENCE HIGH Graphics output
FILE * fileturbh20;		// TURBULENCE HIGH Graphics output
FILE * fileturbh22;		// TURBULENCE HIGH Graphics output
FILE * fileturbh24;		// TURBULENCE HIGH Graphics output
FILE * filelightng;		// LIGHTNING Graphics output
*/
FILE * filecwa;			// Center Weather Advisory

FILE * filenotamjson; 	//notam geojson
FILE * filemetarjson; 	//metar geojson
FILE * fileairmetjson; 	//airmet geojson
FILE * filegairmetjson; //gairmet geojson
FILE * filesigmetjson; 	//sigmet geojson
FILE * filecwajson; 	//sigmet geojson

struct uat_adsb_mdb {
    // presence bits
	int has_sv : 1;
    int has_ms : 1;
    int has_auxsv : 1;

    int position_valid : 1;
    int ns_vel_valid : 1;
    int ew_vel_valid : 1;
    int speed_valid : 1;
    int dimensions_valid : 1;

    //
    // HDR
    //
    uint8_t mdb_type;
    address_qualifier_t address_qualifier;
    uint32_t address;

    //
    // SV
    //

    // if position_valid:
    double lat;
    double lon;

    altitude_type_t altitude_type;
    int32_t altitude; // in feet
    
    uint8_t nic;

    airground_state_t airground_state;

    // if ns_vel_valid:
    int16_t ns_vel; // in kts
    // if ew_vel_valid:
    int16_t ew_vel; // in kts
    
    track_type_t track_type;
    uint16_t track;

    // if speed_valid:
    uint16_t speed; // in kts

    altitude_type_t vert_rate_source;
    int16_t vert_rate; // in ft/min

    // if lengthwidth_valid:
    double length; // in meters (just to be different)
    double width;  // in meters (just to be different)
    int position_offset : 1;  // true if Position Offset Applied

    int utc_coupled : 1;      // true if UTC Coupled flag is set (ADS-B)
    uint8_t tisb_site_id;     // TIS-B site ID, or zero in ADS-B messages
    
    //
    // MS
    //

    uint8_t emitter_category;
    callsign_type_t callsign_type;
    char callsign[9];
    uint8_t emergency_status;
    uint8_t uat_version;
    uint8_t sil;
    uint8_t transmit_mso;
    uint8_t nac_p;
    uint8_t nac_v;
    uint8_t nic_baro;
  
    // capabilities:
    int has_cdti : 1;
    int has_acas : 1;    
    // operational modes:
    int acas_ra_active : 1;
    int ident_active : 1;
    int atc_services : 1;

    heading_type_t heading_type;

    //
    // AUXSV

    altitude_type_t sec_altitude_type;
    int32_t sec_altitude; // in feet
};

//
// Decode/display prototypes
//

void uat_decode_adsb_mdb(uint8_t *frame, struct uat_adsb_mdb *mdb);
void uat_display_adsb_mdb(const struct uat_adsb_mdb *mdb, FILE *to);

//
// UPLINK 
//

// assume 6 byte frames: 2 header bytes, 4 byte payload
// (TIS-B heartbeat with one address, or empty FIS-B APDU)
#define UPLINK_MAX_INFO_FRAMES (424/6)

struct fisb_apdu {
    int a_flag : 1;
    int g_flag : 1;
    int p_flag : 1;
    int s_flag : 1;
    int monthday_valid : 1;
    int seconds_valid : 1;

    uint16_t product_id;
    uint8_t month;   // if monthday_valid
    uint8_t day;     // if monthday_valid
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds; // if seconds_valid

    uint16_t length;
    uint8_t *data;
};

struct uat_uplink_info_frame {
    int is_fisb : 1;

    uint16_t length;
    uint8_t type;
    uint8_t *data; // points within the containing appdata

    // if is_fisb:
    struct fisb_apdu fisb;
};

struct uat_uplink_mdb {
    int position_valid : 1;
    int utc_coupled : 1;
    int app_data_valid : 1;

    // if position_valid:
    double lat;
    double lon;

    uint8_t slot_id;
    uint8_t tisb_site_id;

    // if app_data_valid:
    uint8_t app_data[424];
    unsigned num_info_frames;
    struct uat_uplink_info_frame info_frames[UPLINK_MAX_INFO_FRAMES];    
};

void uat_decode_uplink_mdb(uint8_t *frame, struct uat_uplink_mdb *mdb);
void uat_display_uplink_mdb(const struct uat_uplink_mdb *mdb, FILE *to);

#endif
