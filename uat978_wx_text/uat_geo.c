/*
 * more.c
 *
 *  Created on: Dec 20, 2020
 *      Author: trev
 */
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#include "uat.h"
#include "uat_decode.h"

//#include "metar.h"
#include <time.h>

#include <stdlib.h>

#include "uat_geo.h"


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


