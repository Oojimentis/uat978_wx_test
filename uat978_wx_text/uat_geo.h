/*
 * uat_geo.h
 *
 *  Created on: Dec 20, 2020
 *      Author: TJH
 */

#ifndef UAT_GEO_H_INCLUDED
#define UAT_GEO_H_INCLUDED

#include "metar.h"

void block_location_graphic(int bn, int ns, int sf, double *latN, double *lonW, double *latSize, double *lonSize);

// int  ex3dpoly(FILE *fnm, int rep_count, int rep_num, int alt, char *ob_ele_text);

void graphic_nexrad(const struct fisb_apdu *apdu,FILE *to);

void metar_data( Decoded_METAR *Mptr,FILE *to);


#endif /* UAT_GEO_H_ */
