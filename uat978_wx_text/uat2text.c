//      
// Copyright 2015, Oliver Jowett <oliver@mutability.co.uk>
//

// This file is free software: you may copy, redistribute and/or modify it  
// under the terms of the GNU General Public License as published by the
// Free Software Foundation, either version 2 of the License, or (at your  
// option) any later version.        
//
// This file is distributed in the hope that it will be useful, but  
// WITHOUT ANY WARRANTY; without even the implied warranty of  
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU  
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License  
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <stdio.h>

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "uat.h"
#include "uat_decode.h"
#include "reader.h"

#ifndef NULL
#define NULL   ((void *) 0)
#endif

FILE * filehandle;


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

	//  TJH
	char lyne[80];
	char *item;

	fprintf(stdout, "\nOpening file: uat_gs.txt\n");

	filehandle = fopen("uat_gs.txt","r");

	while (fgets(lyne,80,filehandle)) {
		item = strtok(lyne,"$");
	    strncpy(gs_list[reccount].gs_call,item,5);
	    item = strtok(NULL,"$");
	    strncpy(gs_list[reccount].gs_loc,item,70);
	    reccount++;
	}

	fprintf(stdout, "%d  Records read\n",reccount);
   	fclose(filehandle);

	fprintf(stdout, "\nOpening file: metar.out\n");
		filemetar = fopen("metar.out","w");
		if (filemetar == 0)
		{
			fprintf(stdout,"Error--file could not be opened. \n") ;
				exit (1) ;
		}

		fprintf(stdout, "\nOpening file: notam.out\n");
		filenotam = fopen("notam.out","w");
			if (filenotam == 0)
			{
				fprintf(stdout,"Error--file could not be opened. \n") ;
					exit (1) ;
			}

		fprintf(stdout, "\nOpening file: sua.out\n");
		filesua = fopen("sua.out","w");
			if (filesua == 0)
			{
				fprintf(stdout,"Error--file could not be opened. \n") ;
				exit (1) ;
			}

		fprintf(stdout, "\nOpening file: sigmet.out\n");
		filesigmet = fopen("sigmet.out","w");
			if (filesigmet == 0)
			{
				fprintf(stdout,"Error--file could not be opened. \n") ;
				exit (1) ;
			}

		fprintf(stdout, "\nOpening file: airmet.out\n");
		fileairmet = fopen("airmet.out","w");
			if (fileairmet == 0)
			{
				fprintf(stdout,"Error--file could not be opened. \n") ;
				exit (1) ;
			}

		fprintf(stdout, "\nOpening file: gairmet.out\n");
		filegairmet = fopen("gairmet.out","w");
			if (filegairmet == 0)
			{
				fprintf(stdout,"Error--file could not be opened. \n") ;
				exit (1) ;
			}

	 // TJH

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

