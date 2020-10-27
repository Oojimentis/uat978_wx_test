//      This is a test
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

	seg_count=0;

	fprintf(stdout, "\nOpening file: uat_gs.txt\n\n");

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

   	filemetar = fopen("metar.out","w");
   	if (filemetar == 0)		{
   		fprintf(stdout,"metar.out Error--file could not be opened. \n") ;
   		exit (1) ;		}
   	filenotam = fopen("notam.out","w");
   	if (filenotam == 0)			{
   		fprintf(stdout,"notam.out Error--file could not be opened. \n") ;
   		exit (1) ;			}
   	filesua = fopen("sua.out","w");
   	if (filesua == 0)			{
   		fprintf(stdout,"sua.out Error--file could not be opened. \n") ;
   		exit (1) ;			}
   	filesigmet = fopen("sigmet.out","w");
   	if (filesigmet == 0)			{
   		fprintf(stdout,"sigmet.out Error--file could not be opened. \n") ;
   		exit (1) ;			}
   	fileairmet = fopen("airmet.out ","w");
   	if (fileairmet == 0)			{
   		fprintf(stdout,"airmet.out Error--file could not be opened. \n") ;
   		exit (1) ;			}
   	filegairmet = fopen("gairmet.out ","w");
   	if (filegairmet == 0)			{
   		fprintf(stdout,"gairmet.out Error--file could not be opened. \n") ;
   		exit (1) ;			}
   	filepirep = fopen("pirep.out","w");
   	if (filepirep == 0)			{
   		fprintf(stdout,"pirep.out Error--file could not be opened. \n") ;
   		exit (1) ;			}
   	filenexradc = fopen("nexradc.out","w");
   	if (filenexradc == 0)			{
   		fprintf(stdout,"nexradc.out Error--file could not be opened. \n") ;
   		exit (1) ;			}
   	fileicingl2 = fopen("icingl2.out","w");
   	if (fileicingl2 == 0)			{
   		fprintf(stdout,"icingl2.out Error--file could not be opened. \n") ;
   		exit (1) ;			}
   	fileicingl4 = fopen("icingl4.out","w");
   	if (fileicingl4 == 0)			{
   		fprintf(stdout,"icingl4.out Error--file could not be opened. \n") ;
   		exit (1) ;			}
   	fileicingl6 = fopen("icingl6.out","w");
   	if (fileicingl6 == 0)			{
   		fprintf(stdout,"icingl6.out Error--file could not be opened. \n") ;
   		exit (1) ;			}
   	fileicingl8 = fopen("icingl8.out","w");
   	if (fileicingl8 == 0)			{
   		fprintf(stdout,"icingl8.out Error--file could not be opened. \n") ;
   		exit (1) ;			}
   	fileicingl10 = fopen("icingl10.out","w");
   	if (fileicingl10 == 0)			{
   		fprintf(stdout,"icingl10.out Error--file could not be opened. \n") ;
   		exit (1) ;			}
   	fileicingl12 = fopen("icingl12.out","w");
   	if (fileicingl12 == 0)			{
   		fprintf(stdout,"icingl12.out Error--file could not be opened. \n") ;
   		exit (1) ;			}
   	fileicingl14 = fopen("icingl14.out","w");
   	if (fileicingl14 == 0)			{
   		fprintf(stdout,"icingl14.out Error--file could not be opened. \n") ;
   		exit (1) ;			}
   	fileicingl16 = fopen("icingl16.out","w");
   	if (fileicingl16 == 0)			{
   		fprintf(stdout,"icingl16.out Error--file could not be opened. \n") ;
   		exit (1) ;			}
   	fileicingh18 = fopen("icingh18.out","w");
   	if (fileicingh18 == 0)			{
   		fprintf(stdout,"icingh18.out Error--file could not be opened. \n") ;
   		exit (1) ;			}
   	fileicingh20 = fopen("icingh20.out","w");
   	if (fileicingh20 == 0)			{
   		fprintf(stdout,"icingh20.out Error--file could not be opened. \n") ;
   		exit (1) ;			}
   	fileicingh22 = fopen("icingh22.out","w");
   	if (fileicingh22 == 0)			{
   		fprintf(stdout,"icingh22.out Error--file could not be opened. \n") ;
   		exit (1) ;			}
   	fileicingh24 = fopen("icingh24.out","w");
   	if (fileicingh24 == 0)			{
   		fprintf(stdout,"icingh24.out Error--file could not be opened. \n") ;
   		exit (1) ;			}
   	filecloudt = fopen("cloudt.out","w");
   	if (filecloudt == 0)			{
   		fprintf(stdout,"cloudt.out Error--file could not be opened. \n") ;
   		exit (1) ;			}
   	fileturbl2 = fopen("turbl2.out","w");
   	if (fileturbl2 == 0)			{
   		fprintf(stdout,"turbl2.out Error--file could not be opened. \n") ;
   		exit (1) ;			}
   	fileturbl4 = fopen("turbl4.out","w");
   	if (fileturbl4 == 0)			{
   		fprintf(stdout,"turbl4.out Error--file could not be opened. \n") ;
   		exit (1) ;}
   	fileturbl6 = fopen("turbl6.out","w");
   	if (fileturbl6 == 0)			{
   		fprintf(stdout,"turbl6.out Error--file could not be opened. \n") ;
   		exit (1) ;}
   	fileturbl8 = fopen("turbl8.out","w");
   	if (fileturbl8 == 0)			{
  		fprintf(stdout,"turbl8.out Error--file could not be opened. \n") ;
		exit (1) ;}
	fileturbl10 = fopen("turbl10.out","w");
	if (fileturbl10 == 0)			{
		fprintf(stdout,"turbl10.out Error--file could not be opened. \n") ;
		exit (1) ;}
	fileturbl12 = fopen("turbl12.out","w");
	if (fileturbl12 == 0)			{
		fprintf(stdout,"turbl12.out Error--file could not be opened. \n") ;
		exit (1) ;}
	fileturbl14 = fopen("turbl14.out","w");
	if (fileturbl14 == 0)			{
		fprintf(stdout,"turbl14.out Error--file could not be opened. \n") ;
		exit (1) ;}
	fileturbl16 = fopen("turbl16.out","w");
	if (fileturbl16 == 0)			{
		fprintf(stdout,"turbl16.out Error--file could not be opened. \n") ;
		exit (1) ;}
	fileturbh18 = fopen("turbh18.out","w");
	if (fileturbh18 == 0)			{
	fprintf(stdout,"turbh18.out Error--file could not be opened. \n") ;
		exit (1) ;			}
	fileturbh20 = fopen("turbh20.out","w");
	if (fileturbh20 == 0)			{
		fprintf(stdout,"turbh20.out Error--file could not be opened. \n") ;
		exit (1) ;			}
	fileturbh22 = fopen("turbh22.out","w");
	if (fileturbh22 == 0)			{
		fprintf(stdout,"turbh22.out Error--file could not be opened. \n") ;
		exit (1) ;			}
	fileturbh24 = fopen("turbh24.out","w");
	if (fileturbh24 == 0)			{
		fprintf(stdout,"turbh24.out Error--file could not be opened. \n") ;
		exit (1) ;			}
	filelightng = fopen("lightng.out","w");
	if (filelightng == 0)			{
		fprintf(stdout,"lightng.out Error--file could not be opened. \n") ;
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
