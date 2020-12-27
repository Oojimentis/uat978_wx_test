/*
 * uat_geo.c
 *
 *  Created on: Dec 20, 2020
 *      Author: TJH
 *      Keep graphics and geojson together
 */

#include <stdio.h>
#include <math.h>
#include "uat_decode.h"
#include "uat_geo.h"
#include "asprintf.h"

void graphic_nexrad(const struct fisb_apdu *apdu,FILE *to)
{
   	int scale_factor = 1;
   	int ns_flag = 0;

   	int rle_flag = (apdu->data[0] & 0x80) != 0;
   	int block_num = ((apdu->data[0] & 0x0f) << 16) | (apdu->data[1] << 8) | (apdu->data[2]);
   	int ice_alt = (apdu->data[0] & 0x70) >> 4;
   	int bin=0;

   	char *sql;
	char *zErrMsg = 0;
   	char block[200];
   	char nexrad_time[6];
	char charValue;
	int alt_level = 0;

	switch(apdu->product_id) {
	case 63: case 64:								// ** NEXRAD **************
		scale_factor = (apdu->data[0] & 0x30) >> 4;
		if (apdu->product_id == 64)
			bin = 1;
		break;

	case 70:										// ** Icing Low **************
		switch(ice_alt) {
		case 0:		alt_level = 2000;	break;
		case 1:		alt_level = 4000; 	break;
		case 2:		alt_level = 6000; 	break;
		case 3:		alt_level = 8000; 	break;
		case 4:		alt_level = 10000; 	break;
		case 5:		alt_level = 12000; 	break;
		case 6:		alt_level = 14000; 	break;
		case 7:		alt_level = 16000; 	break;
	    }
	    break;

	case 71:										// ** Icing High **************
		bin = 1;
		switch(ice_alt) {
		case 0:    alt_level = 18000; break;
		case 1:    alt_level = 20000; break;
		case 2:    alt_level = 22000; break;
		case 3:    alt_level = 24000; break;
		}
		break;
//this is a test
	case 84:										// ** Cloud Tops **************
		scale_factor = (apdu->data[0] & 0x30) >> 4;
		break;

	case 90:										// ** Turbulence Low **************
		switch(ice_alt){
		case 0: 	alt_level = 2000; 	break;
		case 1:     alt_level = 4000; 	break;
		case 2:     alt_level = 6000; 	break;
		case 3:     alt_level = 8000; 	break;
		case 4:     alt_level = 10000; 	break;
		case 5:     alt_level = 12000; 	break;
		case 6:     alt_level = 14000; 	break;
		case 7:     alt_level = 16000; 	break;
	    }
	    break;

	case 91:										// ** Turbulence High **************
    	bin = 1;
	    switch(ice_alt){
			case 0:    alt_level = 18000; break;
			case 1:    alt_level = 20000; break;
			case 2:    alt_level = 22000; break;
			case 3:    alt_level = 24000; break;
	    }
	    break;

	case 103:										// ** Lightning  **************
	    	scale_factor = (apdu->data[0] & 0x30) >> 4;
	    	break;
	}

	sprintf(nexrad_time,"%02d%02d",	apdu->hours,apdu->minutes);

    // now decode the bins
    if (rle_flag) {
    	// One bin, 128 values, RLE-encoded
    	double latN = 0;
    	double lonW = 0;
    	double latSize = 0;
    	double lonSize = 0;
    	int num_bins;
    	int ice_prob;

    	block_location_graphic(block_num, ns_flag, scale_factor, &latN, &lonW, &latSize, &lonSize);

    	sprintf(block," ");

    	if (apdu->product_id == 63){
    		asprintf(&sql,"INSERT INTO nexrad (prod_id,nexrad,maptype,maptime,scale_factor,lat_n,lon_w,"
    				"lat_size,lon_size,level,block_data) "
    				"VALUES(%d,'NEXRAD','CONUS','%s',%d,%.0f,%.0f,%.0f,%.0f,%d,",
					apdu->product_id,nexrad_time,
    						scale_factor,latN * 60,lonW * 60,latSize * 60,lonSize * 60,alt_level);
    	}
    	else {
    		asprintf(&sql,"INSERT INTO nexrad (prod_id,nexrad,maptype,maptime,scale_factor,lat_n,lon_w,"
    				"lat_size,lon_size,level,block_data) "
    				"VALUES(%d,'NEXRAD','Regional','%s',%d,%.0f,%.0f,%.0f,%.0f,%d,",
					apdu->product_id,nexrad_time,
    						scale_factor,latN * 60,lonW * 60,latSize * 60,lonSize * 60,alt_level);
    	}

		if (apdu->product_id == 63 || apdu->product_id == 64) {
	    	for (int i = 3; i < apdu->length; ++i) {
	    		int intensity = apdu->data[i] & 7;
	    		int runlength = (apdu->data[i] >> 3) + 1;
	    		while (runlength-- > 0){
	    			charValue=intensity+'0';
	    			sprintf(block,"%s%c",block,charValue);
	    		}
	    	}
	    	asprintf(&sql,"%s '%s')",sql,block);

	    	rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
	    	if( rc != SQLITE_OK ){
	    		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	    		sqlite3_free(zErrMsg);
	    	}
		}
		if (apdu->product_id == 70 || apdu->product_id == 71) {
			for (int i = 3; i < apdu->length; ++i) {
    			num_bins = (apdu->data[i] ) + 1;
    			i = i + 1;
//**    		int sld = apdu->data[i] >> 6;
//**    		int ice_sev = (apdu->data[i]) & 56;
    			ice_prob = (apdu->data[i]) & 7;

    			while (num_bins-- > 0){
    				charValue=ice_prob+'0';
    				sprintf(block,"%s%c",block,charValue);
    			}
			}
	    	asprintf(&sql,"%s '%s')",sql,block);

	    	rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
	    	if( rc != SQLITE_OK ){
	    		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	    		sqlite3_free(zErrMsg);
	    	}
		}
		if (apdu->product_id == 103) {
	       	for (int i = 3; i < apdu->length; ++i) {
//**       		int lgt_pol = apdu->data[i] & 8;
	       		int lgt_cnt = apdu->data[i] & 7;
	       		num_bins = (apdu->data[i] >> 4) + 1;

	       		if (num_bins == 15){
	       			i = i + 1;
	       			num_bins = (apdu->data[i])+1;
	       		}

	       		while (num_bins-- > 0){
    				charValue=lgt_cnt+'0';
    				sprintf(block,"%s%c",block,charValue);
	       		}
	       	}

	    	asprintf(&sql,"%s '%s')",sql,block);

	       	rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
	    	if( rc != SQLITE_OK ){
	    		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	    		sqlite3_free(zErrMsg);
	    	}
		}
    	if (apdu->product_id == 90 || apdu->product_id == 91 || apdu->product_id == 84) {
        	for (int i = 3; i < apdu->length; ++i) {
    			char enc = ' ';
        		int edr_enc  = apdu->data[i] & 15;
        		num_bins = (apdu->data[i] >> 4) + 1;

        		if (num_bins == 15){
        			i = i + 1;
        			num_bins = (apdu->data[i])+1;
        		}
        		switch (edr_enc){
    				case 10:   	enc='a';  	break;
        			case 11:  	enc='b';    break;
        			case 12:  	enc='c'; 	break;
        			case 13:  	enc='d';  	break;
        			case 14: 	enc='e';    break;
        			case 15: 	enc='f';  	break;
        			default:
        				enc = edr_enc +'0';
        			break;
        		}
        		while (num_bins-- > 0){
        			sprintf(block,"%s%c",block,enc);
        		}
        	}
	    	asprintf(&sql,"%s '%s')",sql,block);
	    	rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
	    	if( rc != SQLITE_OK ){
	    		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	    		sqlite3_free(zErrMsg);
	    	}
    	}
    }
    else {    // Empty
   		int row_start;
		int row_offset;
		int row_size;
		int L = apdu->data[3] & 15;

		if (block_num >= 405000) {
   			row_start = block_num - ((block_num - 405000) % 225);
   			row_size = 225;
   		}
		else {
   			row_start = block_num - (block_num % 450);
   			row_size = 450;
   		}
   		row_offset = block_num - row_start;

   		for (int i = 0; i < L; ++i) {
   			int bb;
   			if (i == 0)
   				bb = (apdu->data[3] & 0xF0) | 0x08; // synthesize a first byte in the same format as all the other bytes
   			else
   				bb = (apdu->data[i+3]);

   			for (int j = 0; j < 8; ++j) {
   				if (bb & (1 << j)) {
   					int row_x = (row_offset + 8*i + j - 3) % row_size;
   					int bn = row_start + row_x;
   					double latN = 0, lonW = 0, latSize = 0, lonSize = 0;

   					block_location_graphic(bn, ns_flag, scale_factor, &latN, &lonW, &latSize, &lonSize);

   			    	if (apdu->product_id == 63){
   			    		asprintf(&sql,"INSERT INTO nexrad (prod_id,nexrad,maptype,maptime,scale_factor,lat_n,lon_w,"
   			    				"lat_size,lon_size,level,block_data) "
   			    				"VALUES(%d,'NEXRAD','KONUS','%s',%d,%.0f,%.0f,%.0f,%.0f,%d,",
   								apdu->product_id,nexrad_time,
   			    						scale_factor,latN * 60,lonW * 60,latSize * 60,lonSize * 60,alt_level);
   			    	}
   			    	else {
   			        	asprintf(&sql,"INSERT INTO nexrad (prod_id,nexrad,maptype,maptime,scale_factor,lat_n,lon_w,"
   			        			"lat_size,lon_size,level,block_data) "
   			    				"VALUES(%d,'NEXRAD','Segional','%s',%d,%.0f,%.0f,%.0f,%.0f,%d,",
   								apdu->product_id,nexrad_time,scale_factor,latN * 60,lonW * 60,latSize * 60,lonSize * 60,alt_level);
   			    	}

   			    	sprintf(block," ");
   					for (int k = 0; k < 128; ++k){
	    				charValue=bin+'0';
	    				sprintf(block,"%s%c",block,charValue);
   					}
   			    	asprintf(&sql,"%s '%s')",sql,block);
   			    	rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
   			    	if( rc != SQLITE_OK ){
   			    		fprintf(stderr, "SQL error: %s\n", zErrMsg);
   			    		sqlite3_free(zErrMsg);
   			    	}
   				}
    		}
    	}
    }
    fflush(to);
}

int  ex3dpoly(FILE *fnm, int rep_count, int rep_num, int alt, char *ob_ele_text)
{
	if (rep_count == 0) {
		fprintf(fnm,"{\"type\": \"FeatureCollection\",\n");
		fprintf(fnm,"\"features\": [ \n");
		fprintf(fnm,"{\"type\": \"Feature\", \"properties\": { \"RepNum\": \"%d\", \"Alt\": \"%d\",\"Ob\": \"%s\"},\n",rep_num,alt,ob_ele_text);
		fprintf(fnm,"\"geometry\": { \"type\": \"Polygon\", \"coordinates\": [[\n");
	}
	else {
		fseek(fnm, -3, SEEK_CUR);
		fprintf(fnm,",{\"type\": \"Feature\", \"properties\": { \"RepNum\": \"%d\", \"Alt\": \"%d\",\"Ob\": \"%s\"},\n",rep_num,alt,ob_ele_text);
		fprintf(fnm,"\"geometry\": { \"type\": \"Polygon\", \"coordinates\": [[\n");
	}
	rep_count++;
	return rep_count;

}

void block_location_graphic(int bn, int ns, int sf, double *latN, double *lonW, double *latSize, double *lonSize)
{
#define BLOCK_WIDTH (48.0/60.0)
#define WIDE_BLOCK_WIDTH (96.0/60.0)
#define BLOCK_HEIGHT (4.0/60.0)
#define BLOCK_THRESHOLD 405000
#define BLOCKS_PER_RING 450

	double raw_lat; double raw_lon; double scale;

	if (sf == 1)
		scale = 5.0;
    else if (sf == 2)
        scale = 9.0;
    else
        scale = 1.0;

    if (bn >= BLOCK_THRESHOLD) {
        // 60-90 degrees - even-numbered blocks only
        bn = bn & ~1;
    }

    raw_lat = BLOCK_HEIGHT * trunc(bn / BLOCKS_PER_RING);
    raw_lon = (bn % BLOCKS_PER_RING) * BLOCK_WIDTH;

    *lonSize = (bn >= BLOCK_THRESHOLD ? WIDE_BLOCK_WIDTH : BLOCK_WIDTH) * scale;
    *latSize = BLOCK_HEIGHT * scale;

    // raw_lat/raw_lon points to the southwest corner in the northern hemisphere version
    *lonW = raw_lon;
    if (ns) {
        // southern hemisphere, mirror along the equator
        *latN = 0 - raw_lat;
    }
    else {
        // adjust to the northwest corner
    	*latN = raw_lat + BLOCK_HEIGHT;
    }
}
