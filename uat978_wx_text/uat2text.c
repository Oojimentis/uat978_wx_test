// Copyright 2015, Oliver Jowett <oliver@mutability.co.uk>
//
// UAT2TEXT.C -
//

#include "/usr/include/postgresql/libpq-fe.h"
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
	seg_count = 0;

	char line[75];
	char pg_user[20];
	char pg_pwd[20];
	char pg_db[20];
	char file_path[75];
	char pg_connect[100];

	fileconfig = fopen("config.txt", "r");
	if (fileconfig == 0)	{
		fprintf(stderr, "config.txt Error--file could not be opened. \n");
		exit (1); }
	else {
		fgets(line, 75, fileconfig);
		if (!feof(fileconfig))
			 strcpy(pg_user,line);
		fgets(line, 75, fileconfig);
		if (!feof(fileconfig))
			strcpy(pg_pwd,line);
		fgets(line, 75, fileconfig);
		if (!feof(fileconfig))
			 strcpy(pg_db,line);
		fgets(line, 75, fileconfig);
		if (!feof(fileconfig)){
			 line[strlen(line) - 1] = '\0';
			 strcpy(file_path, line);
		}
	}
	fclose(fileconfig);

// Delete table data...
//		asprintf(&sql,"DELETE from metar; delete from graphic_reports; delete from graphic_coords; delete from text_reports; delete from nexrad;");
//		rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
//		if( rc != SQLITE_OK ){
//			if (rc != 19)
//				fprintf(stderr, "1 SQL error: %s\n", zErrMsg);
//			sqlite3_free(zErrMsg);
//		}
//	}

//   Open Postgresql database
	sprintf(pg_connect, "user=%s password=%s dbname=%s", pg_user, pg_pwd, pg_db);
	conn = PQconnectdb(pg_connect);
	if (PQstatus(conn) == CONNECTION_BAD) {
		fprintf(stderr, "Connection to database failed: %s\n", PQerrorMessage(conn));
		PQfinish(conn);
		exit(1);
	}
	else {
		fprintf(stderr, "Connected to database\n");
	}

	filemetar = fopen("metar.out", "w");
	if (filemetar == 0) {
		fprintf(stderr, "metar.out Error--file could not be opened. \n");
		exit (1); }

	filenotam = fopen("notam.out", "w");
	if (filenotam == 0)	{
		fprintf(stderr, "notam.out Error--file could not be opened. \n");
		exit (1); }

	filesua = fopen("sua.out", "w");
	if (filesua == 0) {
		fprintf(stderr, "sua.out Error--file could not be opened. \n");
		exit (1); }

	filetaf = fopen("taf.out", "w");    // test file for TAF
	if (filetaf == 0) {
		fprintf(stderr, "taf.out Error--file could not be opened. \n");
		exit (1); }

	struct dump978_reader *reader;
	int framecount;
	reader = dump978_reader_new(0, 0);
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
