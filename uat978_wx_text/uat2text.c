// Copyright 2015, Oliver Jowett <oliver@mutability.co.uk>
//
// UAT2TEXT.C
//

#include "uat_decode.h"
#include "reader.h"

#include <stdlib.h>
#include <string.h>
#include "asprintf.h"

#include "parser.h"

#ifndef NULL
#define NULL ((void *) 0)
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
	int reset_database = 1;

	char file_path[75];
	char pg_db[20];
	char pg_pwd[20];
	char pg_user[20];

	char *postsql;


	print_nexrad = 1;

	config_option_t co;
	if ((co = read_config_file("config.txt")) == NULL) {
		perror("read_config_file()");
		return -1;
	}
	while(1) {
		if (strcmp(co->key,"pg_user") == 0)
			strcpy(pg_user,co->value);
		else if (strcmp(co->key,"pg_pwd") == 0)
			strcpy(pg_pwd,co->value);
		else if (strcmp(co->key,"pg_db") == 0)
			strcpy(pg_db,co->value);
		else if (strcmp(co->key,"file_path") == 0)
			strcpy(file_path,co->value);
		else if (strcmp(co->key,"print_nexrad") == 0) {
			if (strcmp(co->value,"N") == 0)
				print_nexrad = 0;
			else if (strcmp(co->value,"Y") == 0)
				print_nexrad = 1;
			else
				fprintf(stderr,"Unknown config Key: %s\nValue: %s\n", co->key, co->value);
		}
		else if (strcmp(co->key,"reset_database") == 0) {
			if (strcmp(co->value,"N") == 0)
				reset_database = 0;
			else if (strcmp(co->value,"Y") == 0)
				reset_database = 1;
			else
				fprintf(stderr,"Unknown config Key: %s\nValue: %s\n", co->key, co->value);
		}
		if (co->prev != NULL) {
			co = co->prev;
		} else {
			break;
		}
	}

// Open Postgresql database
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
	if (reset_database == 1) {
		asprintf(&postsql,"truncate graphics; truncate metar; truncate nexrad; truncate pirep; "
				"truncate sigairmet; truncate taf; truncate circles; truncate sua; "
				"truncate winds; truncate taf_forecast;");

		PGresult *res = PQexec(conn, postsql);
		if (PQresultStatus(res) != PGRES_COMMAND_OK) {
			if (strncmp(PQerrorMessage(conn),"ERROR:  duplicate key", 21) != 0)
				fprintf(stderr, "bad sql %s \nStatus:%d\n", PQerrorMessage(conn),
						PQresultStatus(res));
		}
		PQclear(res);
	}

	filemetar = fopen("metar.out", "w");
	if (filemetar == 0) {
		fprintf(stderr, "metar.out Error--file could not be opened. \n");
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
