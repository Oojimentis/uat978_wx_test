/*
 * uat_geo.c
 *
 *  Created on: Dec 20, 2020
 *      Author: TJH
 *      Keep graphics together
 */

#include <math.h>
#include <string.h>
#include "uat_decode.h"
#include "uat_geo.h"
#include "asprintf.h"

void graphic_nexrad(const struct fisb_apdu *apdu, FILE *to)
{
	char *postsql;
	char nexrad_time[6];
	char *geojson;

	int scale_factor = 1;
	int ns_flag = 0;
	int rle_flag = (apdu->data[0] & 0x80) != 0;
	int block_num = ((apdu->data[0] & 0x0f) << 16) | (apdu->data[1] << 8) | (apdu->data[2]);
	int wx_alt = (apdu->data[0] & 0x70) >> 4;
	int alt_level = 0;

	switch(apdu->product_id) {
	case 63: case 64: case 84: case 103:		// ** NEXRAD/Cloud Top/Lightning
		scale_factor = (apdu->data[0] & 0x30) >> 4;
		break;

	case 70: case 90:							// ** Icing/Turbulence Low
		switch(wx_alt) {
		case 0:		alt_level = 2000;	break;
		case 1:		alt_level = 4000;	break;
		case 2:		alt_level = 6000;	break;
		case 3:		alt_level = 8000;	break;
		case 4:		alt_level = 10000;	break;
		case 5:		alt_level = 12000;	break;
		case 6:		alt_level = 14000;	break;
		case 7:		alt_level = 16000;	break;
	}
		break;

	case 71: case 91:							// ** Icing/Turbulence High
		switch(wx_alt) {
		case 0:		alt_level = 18000;	break;
		case 1:		alt_level = 20000;	break;
		case 2:		alt_level = 22000;	break;
		case 3:		alt_level = 24000;	break;
		}
		break;
	}

	sprintf(nexrad_time, "%02d%02d", apdu->hours, apdu->minutes);

	if (rle_flag) {		// One bin, 128 values, RLE-encoded
		double latN = 0;
		double lonW = 0;
		double latSize = 0;
		double lonSize = 0;
		float t_lat, t_lon;

		int num_bins;
		int ice_sld;
		int ice_prob;
		int ice_sev;
		int intensity;
		int runlength;
		int lgt_cnt;
		int edr_enc;
		int y = 0;
		int x = 0;
		int klen;


		block_location_graphic(block_num, ns_flag, scale_factor, &latN, &lonW, &latSize, &lonSize);

		switch(apdu->product_id) {
		case 63: case 64: {
			int kount = 0;

			for (int i = 3; i < apdu->length; ++i) {
				intensity = apdu->data[i] & 7;
				runlength = (apdu->data[i] >> 3) + 1;
				asprintf(&geojson,"INSERT INTO nexrad (coords, prod_id, intensity, block_num,"
						"maptime, altitude) VALUES ('SRID=4326;GEOMETRYCOLLECTION(");

				while (runlength-- > 0 && intensity > 1) {
					t_lat = latN - (y * (latSize / 4.0));
					t_lon = lonW + (x * (lonSize / 32.0));
					kount++;

					x++;
					if (x == 32) {
						x = 0;
						y++;
					}
					asprintf(&geojson,"%s POINT(%f %f),",geojson, t_lon, t_lat);
				}
				if (kount > 0) {
					klen = strlen(geojson);
					geojson[klen - 1] = ' ';
					kount = 0;
					asprintf(&postsql,"%s)',%d,%d,%d,'%s',%d)",geojson, apdu->product_id,
							intensity, block_num, nexrad_time, alt_level);

					PGresult *res = PQexec(conn, postsql);
					if (PQresultStatus(res) != PGRES_COMMAND_OK) {
						if (strncmp(PQerrorMessage(conn), "ERROR:  duplicate key", 21) != 0)
							fprintf(stderr, "bad sql %s \nStatus:%d\n%s\n", PQerrorMessage(conn),
									PQresultStatus(res), postsql);
					}
					PQclear(res);
				}
			}
		}
		break;

		case 70: case 71: {
			int kount = 0;

			for (int i = 3; i < apdu->length; ++i) {
				num_bins = (apdu->data[i] ) + 1;
				i = i + 1;

				ice_sld = apdu->data[i] >> 6;
				ice_prob = (apdu->data[i]) & 7;
				ice_sev = (apdu->data[i]) >> 3 & 7;

				asprintf(&geojson,"INSERT INTO nexrad (coords, prod_id, intensity, block_num,"
						"maptime, altitude, ice_sld, ice_prob) "
						"VALUES ('SRID=4326;GEOMETRYCOLLECTION(");

				while (num_bins-- > 0 && ice_sev >= 1 && ice_sev < 7) {
					t_lat = latN - (y * (latSize / 4.0));
					t_lon = lonW + (x * (lonSize / 32.0));
					kount++;

					x++;
					if (x == 32) {
						x = 0;
						y++;
					}
					asprintf(&geojson,"%s POINT(%f %f),", geojson, t_lon, t_lat);
				}
				if (kount > 0) {
					klen = strlen(geojson);
					geojson[klen - 1] = ' ';
					kount = 0;
					asprintf(&postsql,"%s)',%d,%d,%d,'%s',%d,%d,%d)", geojson, apdu->product_id, ice_sev,
							 block_num, nexrad_time, alt_level, ice_sld, ice_prob);

					PGresult *res = PQexec(conn, postsql);
					if (PQresultStatus(res) != PGRES_COMMAND_OK) {
						if (strncmp(PQerrorMessage(conn),"ERROR:  duplicate key", 21) != 0)
							fprintf(stderr, "bad sql %s \nStatus:%d\n%s\n", PQerrorMessage(conn),
									PQresultStatus(res), postsql);
					}
					PQclear(res);
				}
			}
		}
		break;

		case 103: {
			int kount = 0;

			for (int i = 3; i < apdu->length; ++i) {
//** 			int lgt_pol = apdu->data[i] & 8;
				lgt_cnt = apdu->data[i] & 7;
				num_bins = (apdu->data[i] >> 4) + 1;

				if (num_bins == 15) {
					i = i + 1;
					num_bins = (apdu->data[i]) + 1;
				}

				asprintf(&geojson,"INSERT INTO nexrad (coords, prod_id, intensity, block_num,"
						"maptime, altitude) VALUES ('SRID=4326;GEOMETRYCOLLECTION(");

				while (num_bins-- > 0 && lgt_cnt >= 1 && lgt_cnt < 7) {
					t_lat = latN - (y * (latSize / 4.0));
					t_lon = lonW + (x * (lonSize / 32.0));
					kount++;

					x++;
					if (x == 32) {
						x = 0;
						y++;
					}
					asprintf(&geojson,"%s POINT(%f %f),", geojson, t_lon, t_lat);
				}
				if (kount > 0) {
					klen = strlen(geojson);
					geojson[klen - 1] = ' ';
					kount = 0;
					asprintf(&postsql,"%s)',%d,%d,%d,'%s',%d)",geojson, apdu->product_id,
							lgt_cnt, block_num, nexrad_time, alt_level);

					PGresult *res = PQexec(conn, postsql);
					if (PQresultStatus(res) != PGRES_COMMAND_OK) {
						if (strncmp(PQerrorMessage(conn),"ERROR:  duplicate key", 21) != 0)
							fprintf(stderr, "bad sql %s \nStatus:%d\n%s\n", PQerrorMessage(conn),
									PQresultStatus(res), postsql);
					}
					PQclear(res);
				}
			}
		}
		break;

		case 90: case 91: {
			int kount = 0;

			for (int i = 3; i < apdu->length; ++i) {
				edr_enc  = apdu->data[i] & 15;
				num_bins = (apdu->data[i] >> 4) + 1;

				if (num_bins == 15) {
					i = i + 1;
					num_bins = (apdu->data[i]) + 1;
				}

				asprintf(&geojson,"INSERT INTO nexrad (coords, prod_id, intensity, block_num,"
						"maptime, altitude) VALUES ('SRID=4326;GEOMETRYCOLLECTION(");

				while (num_bins-- > 0 && edr_enc >= 1 && edr_enc < 15 ) {
					t_lat = latN - (y * (latSize / 4.0));
					t_lon = lonW + (x * (lonSize / 32.0));
					kount++;

					x++;
					if (x >= 32) {
						x = 0;
						y++;
					}
					asprintf(&geojson,"%s POINT(%f %f),", geojson, t_lon, t_lat);
				}
				if (kount > 0) {
					klen = strlen(geojson);
					geojson[klen - 1] = ' ';
					kount = 0;
					asprintf(&postsql,"%s)',%d,%d,%d,'%s',%d)", geojson, apdu->product_id,
							edr_enc, block_num, nexrad_time, alt_level);

					PGresult *res = PQexec(conn, postsql);
					if (PQresultStatus(res) != PGRES_COMMAND_OK) {
						if (strncmp(PQerrorMessage(conn),"ERROR:  duplicate key", 21) != 0)
							fprintf(stderr, "bad sql %s \nStatus:%d\n%s\n", PQerrorMessage(conn),
									PQresultStatus(res), postsql);
					}
					PQclear(res);
				}
			}
		}
		break;

		case 84: {
			int kount = 0;

			for (int i = 3; i < apdu->length; ++i) {
				int flip = 0;
				edr_enc  = apdu->data[i] & 15;
				num_bins = (apdu->data[i] >> 4) + 1;

				if (num_bins == 15) {
					i = i + 1;
					num_bins = (apdu->data[i]) + 1;
				}

				asprintf(&geojson,"INSERT INTO nexrad (coords, prod_id, intensity, block_num,"
						"maptime, altitude) VALUES ('SRID=4326;GEOMETRYCOLLECTION(");

				while (num_bins-- > 0 && edr_enc >= 1 && edr_enc < 15 ) {
					t_lat = latN - (y * (latSize / 4.0));
					t_lon = lonW + (x * (lonSize / 32.0));
					kount++;

					x++;
					if (x >= 32) {
						x = 0;
						y++;
					}
					if (flip % 3 == 0) {		// Reduce resolution.
						asprintf(&geojson,"%s POINT(%f %f),", geojson, t_lon, t_lat);
					}
					flip++;
				}
				if (kount > 0) {
					klen = strlen(geojson);
					geojson[klen - 1] = ' ';
					kount = 0;
					asprintf(&postsql,"%s)',%d,%d,%d,'%s',%d)", geojson, apdu->product_id,
							edr_enc, block_num, nexrad_time, alt_level);

					PGresult *res = PQexec(conn, postsql);
					if (PQresultStatus(res) != PGRES_COMMAND_OK) {
						if (strncmp(PQerrorMessage(conn),"ERROR:  duplicate key", 21) != 0)
							fprintf(stderr, "bad sql %s \nStatus:%d\n%s\n", PQerrorMessage(conn),
									PQresultStatus(res), postsql);
					}
					PQclear(res);
				}
			}
		}
		break;
		}
	}

	fflush(to);
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
//	*lonW = raw_lon;
	if (ns) {
		// southern hemisphere, mirror along the equator
		*latN = 0 - raw_lat;
	}
	else {
		// adjust to the northwest corner
		*latN = raw_lat + BLOCK_HEIGHT;
	}
	if (raw_lon > 180.0) {
		raw_lon = raw_lon - 360.0;
	}

	*lonW = raw_lon;
}

void metar_data( Decoded_METAR *Mptr, FILE *to)
{
	char *postsql;
	char obs_date[10] = " ";
	char temp[10];
	char windSpeed[10];
	char windDir[10];
	char windGust[10];
	char windVar[10];
	char altstng[10];
	char vsbySM[10];
	char SLP[10];
	char dew_pt_temp[10];
	char hrly_precip[10];

	if (Mptr->hourlyPrecip > 1000)
		sprintf(hrly_precip, "- ");
	else
		sprintf(hrly_precip, "%.2f", Mptr->hourlyPrecip);

	if (Mptr->temp > 1000)
		sprintf(temp, "- ");
	else {
		Mptr->temp = Mptr->temp * 9/5  + 32;
		sprintf(temp, "%d", Mptr->temp);
	}

	if (Mptr->dew_pt_temp > 1000)
		sprintf(dew_pt_temp, "-");
	else {
		Mptr->dew_pt_temp = Mptr->dew_pt_temp * 9/5  + 32;
		 sprintf(dew_pt_temp, "%d", Mptr->dew_pt_temp);
	}

	if (Mptr->winData.windSpeed > 1000)
		sprintf(windSpeed, "-");
	else
		sprintf(windSpeed, "%d", Mptr->winData.windSpeed);

	if (Mptr->winData.windDir > 1000)
		sprintf(windDir, "-");
	else
		sprintf(windDir, "%d", Mptr->winData.windDir);

	if (Mptr->winData.windGust > 1000)
		sprintf(windGust, "-");
	else
		sprintf(windGust, "%d", Mptr->winData.windGust);

	if (Mptr->winData.windVRB)
		sprintf(windVar, "Variable");
	else
		sprintf(windVar, " ");

	if (Mptr->inches_altstng > 1000)
		sprintf(altstng, "-");
	else
		sprintf(altstng, "%.2f", Mptr->inches_altstng);

	if (Mptr->prevail_vsbySM > 1000)
		sprintf(vsbySM, "-");
	else
		sprintf(vsbySM, "%.2f", Mptr->prevail_vsbySM);

	if (Mptr->SLP > 5000)
		sprintf(SLP, "-");
	else
		sprintf(SLP, "%.2f", Mptr->SLP);

	sprintf(obs_date, "%02d %02d:%02d", Mptr->ob_date, Mptr->ob_hour, Mptr->ob_minute);

	asprintf(&postsql,"INSERT INTO metar (stn_call, ob_date, temp, windsp, winddir, altimeter, visby,"
			"dewp, hrly_precip, slp, windvar, windgust) "
			"VALUES ('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",
			Mptr->stnid, obs_date, temp, windSpeed,	windDir, altstng, vsbySM, dew_pt_temp,
			hrly_precip, SLP, windVar, windGust);

	PGresult *res = PQexec(conn, postsql);
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		if (strncmp(PQerrorMessage(conn),"ERROR:  duplicate key", 21) != 0)
			fprintf(stderr, "bad sql %s \nStatus:%d\n%s\n", PQerrorMessage(conn),
					PQresultStatus(res), postsql);
	}
	PQclear(res);
}
