// Copyright 2015, Oliver Jowett <oliver@mutability.co.uk>
//
// UAT2TEXT.C
//

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
	}
	else {
		struct uat_uplink_mdb mdb;
		uat_decode_uplink_mdb(frame, &mdb);
		uat_display_uplink_mdb(&mdb, stdout);
	}

	fprintf(stdout, "\n");
	fflush(stdout);
}

int main(int argc, char **argv)
{
	nex_count = 0;
	seg_graph_count = 0;
	seg_text_count = 0;

	char file_path[75];
	char line[75];
	char pg_db[20];
	char pg_pwd[20];
	char pg_user[20];

	char *postsql;

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
		if (!feof(fileconfig)) {
			 line[strlen(line) - 1] = '\0';
			 strcpy(file_path, line);
		}
		fgets(line, 2, fileconfig);
		if (!feof(fileconfig)) {
			char g[2];
			strncpy(g,line,1);
			if (strcmp(g,"N") == 0)
				print_nexrad = 0;
			else
				print_nexrad = 1;
		}
	}
	fclose(fileconfig);

//   Open Postgresql database
	asprintf(&postsql, "user=%s password=%s dbname=%s", pg_user, pg_pwd, pg_db);
	conn = PQconnectdb(postsql);
	if (PQstatus(conn) == CONNECTION_BAD) {
		fprintf(stderr,"                                             \n");
		fprintf(stderr, "*** Connection to database failed: %s\n", PQerrorMessage(conn));
		fprintf(stderr,"                                             \n");
		PQfinish(conn);
		exit(1);
	}
	else {
		fprintf(stderr,"                                             \n");
		fprintf(stderr, "Connected to database: %s\n",pg_db);
	}

// Delete table data...
	asprintf(&postsql,"truncate graphics; truncate metar; truncate nexrad; truncate pirep; "
			"truncate sigairmet; truncate taf; truncate circles; truncate sua; truncate winds;");

	PGresult *res = PQexec(conn, postsql);
		if (PQresultStatus(res) != PGRES_COMMAND_OK) {
			if (strncmp(PQerrorMessage(conn),"ERROR:  duplicate key", 21) != 0)
				fprintf(stderr, "bad sql %s \nStatus:%d\n", PQerrorMessage(conn),
						PQresultStatus(res));
		}
		PQclear(res);

	filemetar = fopen("metar.out", "w");
	if (filemetar == 0) {
		fprintf(stderr, "metar.out Error--file could not be opened. \n");
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
