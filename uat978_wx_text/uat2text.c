// Copyright 2015, Oliver Jowett <oliver@mutability.co.uk>
//
// UAT2TEXT.C -
//

#include <stdio.h>

#include "uat.h"
#include "uat_decode.h"
#include "reader.h"

#include <stdlib.h>
#include <string.h>

#include "asprintf.h"

#ifndef NULL
#define NULL   ((void *) 0)
#endif

void handle_frame(frame_type_t type, uint8_t *frame, int len, void *extra)
{
	if (type == UAT_DOWNLINK) {
		struct uat_adsb_mdb mdb;
        uat_decode_adsb_mdb(frame, &mdb);
        uat_display_adsb_mdb(&mdb, stdout);
    } else {
        struct uat_uplink_mdb mdb;
        uat_decode_uplink_mdb(frame, &mdb);
        uat_display_uplink_mdb(&mdb, stdout);
    }

	fprintf(stdout, "\n");
    fflush(stdout);
}

int main(int argc, char **argv)
 {
	seg_count=0;
	notam_count=0;
	metar_count=0;
	airmet_count=0;
	gairmet_count=0;
	sigmet_count=0;
	cwa_count=0;
	char *zErrMsg = 0;
	char *sql;

// Open sqlite3 database

	rc = sqlite3_open("uat978.db", &db);
	if( rc ) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return(0);
	}
	else {
		fprintf(stderr, "uat978.db - Opened database successfully\n");
// Delete table data...
		asprintf(&sql,"DELETE from metar; delete from graphic_reports; delete from graphic_coords; delete from text_reports; delete from nexrad;");
		rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
		if( rc != SQLITE_OK ){
			if (rc != 19)
				fprintf(stderr, "1 SQL error: %s\n", zErrMsg);

			sqlite3_free(zErrMsg);
		}
	}

   	filenotamjson = fopen("/home/trev/git/map-978/WebContent/notam.geojson","w");
   	if (filenotamjson == 0)		{
   		fprintf(stderr,"notam.geojson Error--file could not be opened. \n") ;
   		exit (1) ;		}
   	fflush(filenotamjson);

   	fileairmetjson = fopen("/home/trev/git/map-978/WebContent/airmet.geojson","w");
   	if (fileairmetjson == 0)		{
   		fprintf(stderr,"airmet.geojson Error--file could not be opened. \n") ;
   		exit (1) ;		}
   	fflush(fileairmetjson);

   	filegairmetjson = fopen("/home/trev/git/map-978/WebContent/gairmet.geojson","w");
   	if (filegairmetjson == 0)		{
   		fprintf(stderr,"gairmet.geojson Error--file could not be opened. \n") ;
   		exit (1) ;		}
   	fflush(filegairmetjson);

   	filesigmetjson = fopen("/home/trev/git/map-978/WebContent/sigmet.geojson","w");
   	if (filesigmetjson == 0)		{
   		fprintf(stderr,"sigmet.geojson Error--file could not be opened. \n") ;
   		exit (1) ;		}
   	fflush(filesigmetjson);

   	filemetarjson = fopen("/home/trev/git/map-978/WebContent/metar.geojson","w");
   	if (filemetarjson == 0)		{
   		fprintf(stderr,"metar.geojson Error--file could not be opened. \n") ;
   		exit (1) ;		}
   	fflush(filemetarjson);

	filecwajson = fopen("/home/trev/git/map-978/WebContent/cwa.geojson","w");
	if (filecwajson == 0)	{
		fprintf(stderr,"cwa.geojson Error--file could not be opened. \n") ;
		exit (1) ;		}
	fflush(filecwajson);

   	filemetar = fopen("metar.out","w");
   	if (filemetar == 0)		{
   		fprintf(stderr,"metar.out Error--file could not be opened. \n") ;
   		exit (1) ;		}

   	filenotam = fopen("notam.out","w");
   	if (filenotam == 0)			{
   		fprintf(stderr,"notam.out Error--file could not be opened. \n") ;
   		exit (1) ;			}

   	filesua = fopen("sua.out","w");
   	if (filesua == 0)			{
   		fprintf(stderr,"sua.out Error--file could not be opened. \n") ;
   		exit (1) ;			}

   	filepirep = fopen("pirep.out","w");
   	if (filepirep == 0)			{
   		fprintf(stderr,"pirep.out Error--file could not be opened. \n") ;
   		exit (1) ;			}

   	filecwa = fopen("cwa.out","w");
	if (filecwa == 0)			{
		fprintf(stderr,"cwa.out Error--file could not be opened. \n") ;
		exit (1) ;			}

	struct dump978_reader *reader;
	int framecount;
	reader = dump978_reader_new(0,0);
	if (!reader) {
		perror("dump978_reader_new");
		return 1;
    }
    
	while ((framecount = dump978_read_frames(reader, handle_frame, NULL)) > 0)
        ;

    if (framecount < 0) {
    	perror("dump978_read_frames");
    	return 1;
    }
    return 0;
}
