/*
* uat_taf.c
*
*	Created on: Jan 14, 2021
*		Author: TH
*		TAF Decode
*/

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "asprintf.h"
#include "uat_decode.h"
#include "uat_taf.h"


#define _POSIX_C_SOURCE 200809L
#include <locale.h>

static unsigned long long int getHash(const char* source)
{
	unsigned long long int hash = 0;

	char c;
	int a;

	if (source == NULL) {
		return 0;
	}

	while (*source != '\0') {
		c = *source++;
		a = c - '0';
		hash = (hash * 10) + a;
	}
	return hash;
}

char* daySuffix(int d)		// adds st, nd, rd, th to day number.
{
	char *date_suff = " ";

	switch (d) {
		case 1: case 21: case 31:	date_suff = "st"; break;
		case 2: case 22:			date_suff = "nd"; break;
		case 3: case 23:			date_suff = "rd"; break;
		default:					date_suff = "th"; break;
	}

	return date_suff;
}

void validDates(char *sd, char *sz, char *ed, char *ez, char *str)
{		//TAF valid dates
	int d;
	char *dt;

	sd[2] = '\0';
	ed[2] = '\0';

	strncpy(sd, str, 2);
	d = atoi(sd);

	dt = daySuffix(d);
	sprintf(sd, "%d%s", d, dt);

	strncpy(sz, str + 2, 2);
	sz[2] = '\0';

	strncpy(ed, str + 5, 2);
	d = atoi(ed);

	dt = daySuffix(d);
	sprintf(ed, "%d%s", d, dt);

	strncpy(ez, str + 7, 2);
	ez[2] = '\0';
}

int err = 0;

char* tafWind(char *temp)
{		// Decode wind/gust.
	char *taf_wind;
	char wind_ret[50];
	char *taf_wind_ret = wind_ret;
	char d[4], s[4], gs[4];
	char kt[3];

	int kt_int = 0;
	int w_len;
	int dir_int = 0;

	gs[0] = '\0';
	if (temp == NULL){
		strcpy(wind_ret, "1-unknown(temp NULL)");
		err = 1;
		return taf_wind_ret;
	}
	w_len = strlen(temp);
	strcpy(wind_ret, "\0");
	if (w_len == 7) {
		strncpy(kt, temp + 5, 2);
		kt[2] = '\0';

		if (strcmp(kt, "KT") != 0) {
			strcpy(wind_ret, "1-unknown(");
			strcat(wind_ret, temp);
			strcat(wind_ret, ") ");
			err = 1;

			return taf_wind_ret;
		}
		else {
			strncpy(d, temp, 3);
			d[3] ='\0';
			strncpy(s, temp + 3, 2);
			s[2] ='\0';
			kt_int = atoi(s);
		}
	}
	else if (w_len == 10) {
		strncpy(kt, temp + 8, 2);
		kt[2] = '\0';
		if (strcmp(kt, "KT") != 0) {
			strcpy(wind_ret, "2-unknown(");
			strcat(wind_ret, temp);
			strcat(wind_ret, ") ");
			err = 1;

			return taf_wind_ret;
		}
		else {
			strncpy(d, temp, 3);
			d[3] = '\0';
			strncpy(s, temp + 3, 2);
			s[2] = '\0';
			kt_int = atoi(s);
			strncpy(gs, temp + 6, 2);
			gs[2] = '\0';
		}
	}
	else {
		strcpy(wind_ret, "8-unknown(");
		strcat(wind_ret, temp);
		strcat(wind_ret, ") ");
		err = 1;
		return taf_wind_ret;
	}
	if (strcmp(d, "VRB") == 0)
		asprintf(&taf_wind, "Variable, speed: %dkt", kt_int);
	else {
		dir_int = atoi(d);
		asprintf(&taf_wind, "Direction: %d°, Speed: %dkt", dir_int, kt_int);
	}
	if (strcmp(gs, "\0") == 0) {
		if (strcmp(temp, "00000KT") == 0)
			strcat(wind_ret, "Calm");
		else
			strcat(wind_ret, taf_wind);
	}
	else {
		strcat(wind_ret, taf_wind);
		strcat(wind_ret, ", Gusts: ");
		strcat(wind_ret, gs);
		strcat(wind_ret, "kt");
	}
	return taf_wind_ret;
}

void tafVisibilty(char *temp, char *taf_vis, char *temp2)
{		// Decode visibility.
	int len_v, sm = 0;
	char vis[10];

	taf_vis[0] = '\0';

	if (temp == NULL)
		return;
	len_v = strlen(temp);

	if (len_v == 1) {
		sprintf(taf_vis, "%s", temp);
		len_v = strlen(temp2);
		asprintf(&temp, "%s %s", temp, temp2);
		len_v = strlen(temp);
	}

	if (strncmp(temp, "P", 1) == 0) {
		strncpy(vis, temp + 1, len_v - 3);
		sm = atoi(vis);
		if (sm == 1)
			sprintf(taf_vis, "> %d statue mile", sm);
		else
			sprintf(taf_vis, "> %d statue miles", sm);
	}
	else if (strncmp(temp, "M", 1) == 0) {
		strncpy(vis, temp + 1, len_v - 3);
		sm=atoi(vis);
		if (sm == 1)
			sprintf(taf_vis, "> %d statue mile", sm);
		else
			sprintf(taf_vis, "> %d statue miles", sm);
	}
	else if (strncmp(temp + (len_v - 2), "SM", 2) == 0) {
		strncpy(vis, temp, len_v - 2);
		sm=atoi(vis);
		vis[len_v - 2] = '\0';
		if (sm == 1)
			sprintf(taf_vis, "%s statue mile", vis);
		else
			sprintf(taf_vis, "> %d statue miles", sm);
	}
	else if (strncmp(temp, "9999", 4) == 0) {
		sprintf(taf_vis, "Clear");
	}
	else if(isdigit(temp[0])) {
		sprintf(taf_vis, "%s meters ", temp);
	}
	else {
		sprintf(taf_vis, " Unknown vis (%s)", temp);
		err = 1;
	}
}

char* tafWeather(char *taf_list)
{		// Decode weather/sky conditions.
	static char taf_wx_all[500];

	char *temp;
	char temp2[30];
	char *taf_wind;
	char taf_wx[200];
	char wx_int[50];
	char *dt;
	char sd[5], sz[3], ed[5], ez[3];
	char perc[3];

	int d;
	int found;
	int l;
	int len_t;
	int units;
	int vis_met = 0;
	int wx1, wx2, wx3;
	int prb_no_wind_vis = 0;

	float fahr;
	float units_f;

	temp = strsep(&taf_list, " ");
	taf_wx[0] = '\0';
	taf_wx_all[0] = '\0';

	while (temp != NULL) {
		found = 1;
		l = strlen(temp);

		switch(getHash(temp)) {
		case AUTOMATED:	strcat(taf_wx_all, "Automated ");break;
		case BC:		strcat(taf_wx_all, "Patches, "); break;
		case BL:		strcat(taf_wx_all, "Blowing, "); break;
		case BR:		strcat(taf_wx_all, "Mist, "); break;
		case DR:		strcat(taf_wx_all, "Low drifting, "); break;
		case DS:		strcat(taf_wx_all, "Duststorm, "); break;
		case DU:		strcat(taf_wx_all, "Widespread dust, "); break;
		case DZ:		strcat(taf_wx_all, "Drizzle, "); break;
		case HZ:		strcat(taf_wx_all, "Haze, "); break;
		case FC:		strcat(taf_wx_all, "Funnel clouds, "); break;
		case FG:		strcat(taf_wx_all, "Fog, "); break;
		case FU:		strcat(taf_wx_all, "Smoke, "); break;
		case GRHAIL:	strcat(taf_wx_all, "Hail, "); break;
		case GS:		strcat(taf_wx_all, "Small hail and/or snow pellets, "); break;
		case IC:		strcat(taf_wx_all, "Ice crystals, "); break;
		case MI:		strcat(taf_wx_all, "Shallow, "); break;
		case NSW:		strcat(taf_wx_all, "No significant weather, "); break;
		case PL:		strcat(taf_wx_all, "Ice pellets, "); break;
		case PO:		strcat(taf_wx_all, "Dust/sand whirls, "); break;
		case PR:		strcat(taf_wx_all, "Partial, "); break;
		case RA:		strcat(taf_wx_all, "Rain, "); break;
		case SA:		strcat(taf_wx_all, "Sand, "); break;
		case SG:		strcat(taf_wx_all, "Snow grains, "); break;
		case SN:		strcat(taf_wx_all, "Snow, "); break;
		case SQ:		strcat(taf_wx_all, "Squall(s), "); break;
		case SS:		strcat(taf_wx_all, "Sandstorm, "); break;
		case TS:		strcat(taf_wx_all, "Thunderstorm, "); break;
		case UP:		strcat(taf_wx_all, "Unknown precipitation, "); break;
		case VA:		strcat(taf_wx_all, "Volcanic ash, "); break;
		case VCFG:		strcat(taf_wx_all, "Fog in vicinity, "); break;
		case VCSH:		strcat(taf_wx_all, "Showers in vicinity, "); break;
		case VCTS:		strcat(taf_wx_all, "Thunderstorm in vicinity, "); break;

		case BCFG:		strcat(taf_wx_all, "Fog patches, "); break;
		case BLSN:		strcat(taf_wx_all, "Blowing snow, "); break;
		case DRSN:		strcat(taf_wx_all, "Drizzle/snow, "); break;
		case FZFG:		strcat(taf_wx_all, "Freezing fog, "); break;
		case FZRA:		strcat(taf_wx_all, "Freezing rain, "); break;
		case MIFG:		strcat(taf_wx_all, "Shallow fog, "); break;
		case RAPL:		strcat(taf_wx_all, "Rain/ice pellets, "); break;
		case RASN:		strcat(taf_wx_all, "Rain/snow, "); break;
		case SHRA:	case SHRN: 	strcat(taf_wx_all, "Rain showers, "); break;
		case SHSN:		strcat(taf_wx_all, "Showers/snow, "); break;
		case SNPL:		case PLSN:	strcat(taf_wx_all, "Snow/ice pellets, "); break;
		case TSRA:		strcat(taf_wx_all, "Thunder/rain, "); break;
		case TSSN:		strcat(taf_wx_all, "Thunder/snow, "); break;

		case FZRAPL:	strcat(taf_wx_all, "Freezing rain/ice pellets, "); break;
		case FZRASN:	strcat(taf_wx_all, "Freezing rain/snow, "); break;
		case RASNPL:	strcat(taf_wx_all, "Rain/snow/ice pellets, "); break;
		case SHRADZ:	strcat(taf_wx_all, "Showers/rain/drizzle, "); break;
		case SHRASN:	strcat(taf_wx_all, "Showers/rain/snow, "); break;
		case TSRAGR:	strcat(taf_wx_all, "Thunder/rain/hail, "); break;

		case FZRASNPL:	strcat(taf_wx_all, "Freezing rain/snow/ice pellets, "); break;

		case LDZ:		strcat(taf_wx_all, "Light drizzle, "); break;
		case LPL:		strcat(taf_wx_all, "Light ice pellets, "); break;
		case LRA:		strcat(taf_wx_all, "Light rain, "); break;
		case LSN:		strcat(taf_wx_all, "Light snow, "); break;

		case LFZDZ:		strcat(taf_wx_all, "Light freezing drizzle, "); break;
		case LFZRA:	case LFRZA:		strcat(taf_wx_all, "Light freezing rain, "); break;
		case LPLRA:		strcat(taf_wx_all, "Light ice pellets/rain, "); break;
		case LRADZ:		strcat(taf_wx_all, "Light rain/drizzle, "); break;
		case LRAPL:		strcat(taf_wx_all, "Light rain/ice pellets, "); break;
		case LSHRA:		strcat(taf_wx_all, "Light rain showers, "); break;
		case LSHSN:		strcat(taf_wx_all, "Light snow shower, "); break;
		case LSNDZ:	case LDZSN:		strcat(taf_wx_all, "Light snow/drizzle, "); break;
		case LTSRA:		strcat(taf_wx_all, "Light thunder/rain, "); break;

		case LFZDZPL:	strcat(taf_wx_all, "Light freezing drizzle/ice pellets, "); break;
		case LFZDZSN:	strcat(taf_wx_all, "Light freezing snow/drizzle, "); break;
		case LFZRADZ:	strcat(taf_wx_all, "Light freezing rain/drizzle, "); break;
		case LFZRAPL:	strcat(taf_wx_all, "Light freezing rain/ice pellets, "); break;
		case LFZRASN:	strcat(taf_wx_all, "Light freezing rain/snow, "); break;
		case LRASNPL:	strcat(taf_wx_all, "Light rain/snow/ice pellets, "); break;
		case LSHRADZ:	strcat(taf_wx_all, "Light showers rain/drizzle, "); break;
		case LSHRAPL:	strcat(taf_wx_all, "Light showers rain/ice pellets, "); break;
		case LSHRASN: case LSHSNRA: strcat(taf_wx_all, "Light showers rain/snow, "); break;
		case LTSRAGR:	strcat(taf_wx_all, "Light thunder/rain/hail, "); break;
		case LFZDZSNPL:	strcat(taf_wx_all, "Light freezing drizzle/snow/ice pellets, "); break;

		case LSHRAPLSN:	strcat(taf_wx_all, "Light showers rain/ice pellets/snow, "); break;

		case LSNPL:		case LPLSN:		strcat(taf_wx_all, "Light snow/ice pellets, "); break;
		case LFZRAPLSN:	case LFZRASNPL:	strcat(taf_wx_all, "Light freezing rain/ice pellets/snow, "); break;
		case LRASN:		case LSNRA: 	strcat(taf_wx_all, "Light rain/snow, "); break;

		case HSN:		strcat(taf_wx_all, "Heavy snow, "); break;
		case HRA:		strcat(taf_wx_all, "Heavy rain, "); break;

		case HSHRA:		strcat(taf_wx_all, "Heavy rain showers, "); break;
		case HSHSN:		strcat(taf_wx_all, "Heavy snow showers, "); break;
		case HTSRA:		strcat(taf_wx_all, "Heavy thunder/rain showers, "); break;

		case HTSRAGR:	strcat(taf_wx_all, "Heavy thunder/rain/hail, "); break;

		default:	found = 0; break;
		}
		if (found == 0) {
			if (strncmp(temp, "BKN", 3) == 0) {
				units = atoi(temp + 3);
				if (l > 6) {
					strncpy(temp2, temp + 6, l - 6);
					temp2[2] = '\0';
					if (strcmp(temp2, "CB") == 0) {
						sprintf(taf_wx, "Broken cumulonimbus clouds (%'dft) ", units * 100);
						strcat(taf_wx_all, taf_wx);
					}
				}
				else {
					sprintf(taf_wx, "Broken clouds (%'dft) ", units * 100);
					strcat(taf_wx_all, taf_wx);
				}
			}
			else if (strncmp(temp, "FEW", 3) == 0) {
				units = atoi(temp + 3);
				if (l > 6) {
					strncpy(temp2, temp + 6, l - 6);
					temp2[2] = '\0';
					if (strcmp(temp2, "CB") == 0) {
						sprintf(taf_wx, "Few cumulonimbus clouds (%'dft) ", units * 100);
						strcat(taf_wx_all, taf_wx);
					}
				}
				else {
					sprintf(taf_wx, "Few clouds (%'dft) ", units * 100);
					strcat(taf_wx_all, taf_wx);
				}
			}
			else if (strncmp(temp, "OVC", 3) == 0) {
				units = atoi(temp + 3);
				if (l > 6) {
					strncpy(temp2, temp + 6, l - 6);
					temp2[2] = '\0';
					if (strcmp(temp2, "CB") == 0) {
						sprintf(taf_wx, "Overcast cumulonimbus clouds (%'dft) ", units * 100);
						strcat(taf_wx_all, taf_wx);
					}
				}
				else {
					sprintf(taf_wx, "Overcast (%'dft) ", units * 100);
					strcat(taf_wx_all, taf_wx);
				}
			}
			else if (strncmp(temp, "SCT", 3) == 0) {
				units = atoi(temp + 3);
				if (l > 6) {
					strncpy(temp2, temp + 6 , l - 6);
					temp2[2] = '\0';
					if (strcmp(temp2, "CB") == 0) {
						sprintf(taf_wx, "Scattered cumulonimbus clouds (%'dft) ", units * 100);
						strcat(taf_wx_all, taf_wx);
					}
				}
				else {
					sprintf(taf_wx, "Scattered clouds (%'dft) ", units * 100);
					strcat(taf_wx_all, taf_wx);
				}
			}
			else if (strncmp(temp, "SKC", 3) == 0) {
				sprintf(taf_wx, "Sky clear ");
				strcat(taf_wx_all, taf_wx);
			}
			else if (strncmp(temp, "AFT", 3) == 0) {
				strcat(taf_wx_all, " after ");
				temp = strsep(&taf_list, " ");
				strcat(taf_wx_all,temp);
				strcat(taf_wx_all," ");
			}
			else if ((strncmp(temp, "AMD", 3) == 0) && (strlen(temp) == 3)) {
				sprintf(taf_wx, "Amendment ");
				strcat(taf_wx_all, taf_wx);
			}
			else if (strncmp(temp, "AMDS", 4) == 0) {
				strcat(taf_wx_all, "amendments ");
			}
			else if (strncmp(temp, "AMENDS", 6) == 0) {
				strcat(taf_wx_all, "amendments ");
			}
			else if (strncmp(temp, "AND", 3) == 0) {
				strcat(taf_wx_all, "and ");
			}
			else if (strncmp(temp, "CLD", 3) == 0) {
				strcat(taf_wx_all, "cloud ");
			}
			else if (strncmp(temp, "COR", 3) == 0) {
				strcat(taf_wx_all, "Corrected ");
				temp = strsep(&taf_list, " ");
				strcat(taf_wx_all, temp);
				strcat(taf_wx_all, " ");
			}
			else if (strncmp(temp, "(INCMPL)", 8) == 0) {
				strcat(taf_wx_all, "(Report incomplete)");
			}
			else if (strncmp(temp, "LAST", 4) == 0) {
				strcat(taf_wx_all, "Last: ");
			}
			else if (strncmp(temp, "LTD", 3) == 0) {
				strcat(taf_wx_all, "limited ");
			}
			else if (strncmp(temp, "METWATCH", 8) == 0) {
				strcat(taf_wx_all, "Metwatch ");
				temp = strsep(&taf_list, " ");
				strcat(taf_wx_all, temp);
				strcat(taf_wx_all, " ");
			}
			else if (strncmp(temp, "NEXT", 4) == 0) {
				strcat(taf_wx_all, "next ");
				temp = strsep(&taf_list, " ");
				strcat(taf_wx_all,temp);
			}
			else if ((strncmp(temp, "NO", 2) == 0) && (strlen(temp) == 2)) {
				strcat(taf_wx_all, "no ");
			}
			else if ((strncmp(temp, "NOT", 3) == 0) && (strlen(temp) == 3)) {
				sprintf(taf_wx, "not ");
				strcat(taf_wx_all, taf_wx);
			}
			else if (strncmp(temp, "SENSOR", 6) == 0) {
				strcat(taf_wx_all, "sensor ");
			}
			else if ((strncmp(temp, "SKED", 4) == 0) && (strlen(temp) == 4)) {
				sprintf(taf_wx, "Scheduled ");
				strcat(taf_wx_all, taf_wx);
			}
			else if (strncmp(temp, "TIL", 3) == 0) {
				strcat(taf_wx_all, "until ");
				temp = strsep(&taf_list, " ");
				strcat(taf_wx_all, temp);
				strcat(taf_wx_all, " ");
			}
			else if (strncmp(temp, "TO", 2) == 0) {
				strcat(taf_wx_all, "to ");
			}
			else if (strncmp(temp, "UNTIL", 5) == 0) {
				strcat(taf_wx_all, "until ");
				temp = strsep(&taf_list, " ");
				strcat(taf_wx_all, temp);
				strcat(taf_wx_all, " ");
			}
			else if (strncmp(temp, "VIS", 3) == 0) {
				strcat(taf_wx_all, "visibility ");
			}
			else if (strncmp(temp, "WIND", 4) == 0) {
				strcat(taf_wx_all, "wind ");
			}
			else if (strncmp(temp, "WND", 3) == 0) {
				strcat(taf_wx_all, "Wind: ");
				temp = strsep(&taf_list, " ");
				taf_wind = tafWind(temp);
				strcat(taf_wx_all, taf_wind);
			}
			else if (strncmp(temp, "WX", 2) == 0) {
				strcat(taf_wx_all, "weather ");
			}
			else if (strncmp(temp, "QNH", 3) == 0) {
				strncpy(temp2, temp + 3, 4);
				units_f = atof(temp2);
				sprintf(taf_wx, "Altimeter %2.2fHg ", units_f/100);
				strcat(taf_wx_all, taf_wx);
			}
			else if ((strncmp(temp, "6", 1) == 0) && (strlen(temp) == 6)) {
				units = strlen(temp);
				if (units == 6) {
					wx_int[0] = '\0';
					strncpy(temp2, temp + 1, 1);
					temp2[1] = '\0';
					wx1 = atoi(temp2);

					switch(wx1) {
					case 0: strcpy(wx_int, "Trace icing "); break;
					case 1: strcpy(wx_int, "Light mixed icing "); break;
					case 2: strcpy(wx_int, "Light rime icing in cloud "); break;
					case 3: strcpy(wx_int, "Light clear icing in precipitation "); break;
					case 4: strcpy(wx_int, "Moderate mixed icing "); break;
					case 5: strcpy(wx_int, "Moderate rime Icing In Cloud "); break;
					case 6: strcpy(wx_int, "Moderate clear icing in precipitation "); break;
					case 7: strcpy(wx_int, "Severe mixed icing "); break;
					case 8: strcpy(wx_int, "Severe rime icing in cloud "); break;
					case 9: strcpy(wx_int, "Severe clear icing in precipitation "); break;
					}
					strncpy(temp2, temp + 2, 3);
					temp2[3] = '\0';
					wx2 = (atoi(temp2) * 100);
					strncpy(temp2, temp + 5, 1);
					temp2[1] = '\0';
					wx3 = (atoi(temp2) * 1000);

					sprintf(taf_wx, "%s %dft-%dft ", wx_int, wx2, wx2 + wx3);
					strcat(taf_wx_all, taf_wx);
				}
				else {
					sprintf(taf_wx, " (3-unknown %s) ", temp);
					err = 1;
					strcat(taf_wx_all, taf_wx);
				}
			}
			else if (strncmp(temp, "VV", 2) == 0) {
				strcpy(temp2, temp + 2);
				units = (atoi(temp2) * 100);
				sprintf(taf_wx, "Verical visibilty %dft ", units);
				strcat(taf_wx_all, taf_wx);
			}
			else if (strncmp(temp, "TNM", 3) == 0) {
				strncpy(temp2, temp + 3, 2);
				temp2[2] = '\0';
				units = (atoi(temp2) * -1);
				fahr = (units * 9/5) + 32;
				strncpy(temp2, temp + 6, 2);
				d = atoi(temp2);
				dt = daySuffix(d);

				strncpy(temp2, temp + 8, 2);
				temp2[2] = '\0';

				sprintf(taf_wx, "Min temp %.1f℉ on %d%s @%s:00z ", fahr, d, dt, temp2);
				strcat(taf_wx_all, taf_wx);
			}
			else if ((strncmp(temp, "TX", 2) == 0) || (strncmp(temp, "TN", 2) == 0)) {
				if (temp[2] == 'M') {
					strncpy(temp2, temp + 3, 2);
					temp2[2] = '\0';
					units = (atoi(temp2) * -1);
					fahr = (units * 9/5) + 32;
				}
				else {
					strncpy(temp2, temp + 2, 2);
					temp2[2] = '\0';
					units = atoi(temp2);
					fahr = (units * 9/5) + 32;
				}

				if (temp[2] == 'M')
					strncpy(temp2, temp + 6, 2);
				else
					strncpy(temp2, temp + 5, 2);

				d = atoi(temp2);
				dt = daySuffix(d);

				strncpy(temp2, temp + 7, 2);
				temp2[2] = '\0';

				if (strncmp(temp, "TX", 2) == 0) {
					sprintf(taf_wx, "Max Temp %.1f℉ on %d%s %s:00z ", fahr, d, dt, temp2);
					strcat(taf_wx_all, taf_wx);
				}
				else {
					sprintf(taf_wx, "Min Temp %.1f℉ on %d%s %s:00z ", fahr, d, dt, temp2);
					strcat(taf_wx_all, taf_wx);
				}
			}
			else if ((strncmp(temp, "T", 1) == 0) && ((strncmp(temp + 3, "/", 1) == 0) ||
					(strncmp(temp + 4, "/", 1) == 0))) {

				if (temp[1] == 'M') {
					strncpy(temp2, temp + 2, 2);
					temp2[2] = '\0';
					units = (atoi(temp2) * -1);
					fahr = (units * 9/5) + 32;
					strncpy(temp2, temp + 5, 2);
					d = atoi(temp2);
				}
				else {
					strncpy(temp2, temp + 1, 2);
					temp2[2] = '\0';
					units = atoi(temp2);
					fahr = (units * 9/5) + 32;
					strncpy(temp2, temp + 4, 2);
					d = atoi(temp2);
				}
				dt = daySuffix(d);

				if (temp[1] == 'M')
					strncpy(temp2, temp + 7, 2);
				else
					strncpy(temp2, temp + 6, 2);

				temp2[2] = '\0';

				sprintf(taf_wx, "Temperature %.1f℉ on %d%s %s:00z ", fahr, d, dt, temp2);
				strcat(taf_wx_all, taf_wx);
			}
			else if (strncmp(temp, "WS", 2) == 0)	{
				strncpy(temp2, temp + 2, 3);
				units = (atoi(temp2) * 100);
				len_t = strlen(temp);
				strncpy(temp2, temp + 6, len_t - 6);
				taf_wind = tafWind(temp2);

				sprintf(taf_wx, "Wind shear @ %'dft %s ", units, taf_wind);
				strcat(taf_wx_all, taf_wx);
			}
			else if (strncmp(temp, "PROB", 4) == 0) {
				strncpy(perc, temp + 4, 2);
				perc[2] = '\0';
				temp = strsep(&taf_list, " ");
				validDates(sd, sz, ed, ez, temp);
				sprintf(taf_wx, "%s%% Probability %s %s:00z-%s %s:00z ",perc, sd, sz, ed, ez);
				strcat(taf_wx_all, taf_wx);

				temp = strsep(&taf_list, " ");
				if (strncmp(temp + (strlen(temp) - 2), "KT", 2) == 0) {
					taf_wind = tafWind(temp);
					strcat(taf_wx_all, "Wind: ");
					strcat(taf_wx_all, taf_wind);
					strcat(taf_wx_all, " ");
					temp = strsep(&taf_list, " ");
				}
				else if (strncmp(temp + (strlen(temp) - 2), "SM", 2) == 0) {
					tafVisibilty(temp, temp2, taf_list);
					sprintf(taf_wx, " Visibility: %s", temp2);
					strcat(taf_wx_all, taf_wx);
					prb_no_wind_vis = 0;
				}
				else
					prb_no_wind_vis = 1;
			}
			else if (strlen(temp) == 4) {
				vis_met = 0;
				for (int k = 0; k < 4; ++k) {
					if (isdigit(temp[k]))
						vis_met++;
				}
				if (vis_met == 4) {
					temp2[0] = '\0';
					tafVisibilty(temp, temp2, "  ");
					sprintf(taf_wx, "<b>Visibility:</b> %s", temp2);
					strcat(taf_wx_all, taf_wx);
				}
				else if ((strncmp(temp, "SKED", 4) == 0) && (strlen(temp) == 4)) {
					sprintf(taf_wx, "scheduled ");
					strcat(taf_wx_all, taf_wx);
					temp = strsep(&taf_list, " ");
					if (temp != NULL) {
						strcat(taf_wx_all, temp);
						}
					}
				else {
					sprintf(taf_wx, " 6-Unknown (%s) ", temp);
					err = 1;
					strcat(taf_wx_all, taf_wx);
				}
			}
			else if (strncmp(temp, "VRB", 3) == 0) {
				taf_wind = tafWind(temp);
				sprintf(taf_wx, "<b>Wind:</b> %s ", taf_wind);
				strcat(taf_wx_all, taf_wx);
			}
			else if ((strncmp(temp, "FN", 2) == 0) && (strlen(temp) == 7)) {
				sprintf(taf_wx, "From Fleet Norfolk (%s) ", temp);
				strcat(taf_wx_all, taf_wx);
			}
			else if ((strncmp(temp, "5", 1) == 0) && (strlen(temp) == 6)) {
				units = strlen(temp);
				if (units == 6) {
					wx_int[0] = '\0';
					strncpy(temp2, temp + 1, 1);
					temp2[1] = '\0';
					if (temp2[0] == 'X')
						strcpy(wx_int, "Extreme turbulence ");
					else {
						wx1 = atoi(temp2);

						switch(wx1) {
						case 0: strcpy(wx_int, "No turbulence "); break;
						case 1: strcpy(wx_int, "Light turbulence "); break;
						case 2: strcpy(wx_int, "Moderate turbulence in clear air, occasional "); break;
						case 3: strcpy(wx_int, "Moderate turbulence in clear air, frequent "); break;
						case 4: strcpy(wx_int, "Moderate turbulence in cloud, occasional "); break;
						case 5: strcpy(wx_int, "Moderate turbulence in cloud, frequent "); break;
						case 6: strcpy(wx_int, "Severe turbulence in clear air, occasional "); break;
						case 7: strcpy(wx_int, "Severe turbulence in clear air, frequent "); break;
						case 8: strcpy(wx_int, "Severe turbulence in cloud, occasional "); break;
						case 9: strcpy(wx_int, "Severe turbulence in cloud, frequent "); break;
						}
					}
					strncpy(temp2, temp + 2, 3);
					temp2[3] = '\0';
					wx2 = (atoi(temp2) * 100);
					strncpy(temp2, temp + 5, 1);
					temp2[1] = '\0';
					wx3 = (atoi(temp2) * 1000);

					sprintf(taf_wx, "%s %dft-%dft ", wx_int, wx2, wx2 + wx3);
					strcat(taf_wx_all, taf_wx);
				}
				else {
					sprintf(taf_wx, "(4-unknown %s) ", temp);
					err = 1;
					strcat(taf_wx_all, taf_wx);
				}
			}
			else if (strlen(temp) == 5) {
				if (temp[4] == 'Z') {
					strcat(taf_wx_all, temp);
					strcat(taf_wx_all, " ");
				}
				else {
					sprintf(taf_wx, "9-Unknown (%s) ", temp);
					err = 1;
					strcat(taf_wx_all, taf_wx);
				}
			}
			else if (strncmp(temp + 4, "/", 1) == 0 && strlen(temp) == 9) {
				validDates(sd, sz, ed, ez, temp);
				strcat(taf_wx_all, sd);
				strcat(taf_wx_all, " @");
				strcat(taf_wx_all, sz);
				strcat(taf_wx_all, ":00z-");
				strcat(taf_wx_all, ed);
				strcat(taf_wx_all, " @");
				strcat(taf_wx_all, ez);
				strcat(taf_wx_all, ":00z");
			}
			else {
				sprintf(taf_wx, "5-Unknown (%s) ", temp);
				err = 1;
				strcat(taf_wx_all, taf_wx);
			}
		}

		if (prb_no_wind_vis == 0)
			temp = strsep(&taf_list, " ");
		else
			prb_no_wind_vis = 0;
	}
return taf_wx_all;
}

void taf_decode(char *taf_linzs,char *issued, char *reptime, char *gstn, int taf_line_num, FILE *to)
{		// TAF Decode
	char current_all[100];
	char fsz[6];
	char fsz_hr[3];
	char fsz_min[3];

	char mil[1];
	char sd[10], sz[3], ed[5], ez[3];
	char taf_t[5];
	char taf_cld[50];
	char wind[50];
	char *dt;
	char postsql[1000];
	char *t_temp;
	char *taf_condx;
	char *taf_lines;
	char *taf_temp;
	char *temp, *temp2;
	char *visby;
	char hold[2000];
	char fc_curr_wind[50];
	int dx;
	int nil = 0;
	int sw = 0;
	int temp_len;

	setlocale(LC_ALL, "");

	asprintf(&taf_lines, "%s", taf_linzs);
// Valid dates
	strncpy(taf_t, taf_lines, 2);
	taf_t[2] = '\0';

	err= 0;
	temp = strsep(&taf_lines, " ");

	if ((isdigit(taf_t[0])) && (strlen(temp) != 6) && (temp[4] != 'Z')) {	// (current) 1st line?
		mil[0] = temp[4];

		if (mil[0] == '/') {
			validDates(sd, sz, ed, ez, temp);
			strcpy(current_all, sd);
			strcat(current_all, " @");
			strcat(current_all, sz);
			strcat(current_all, ":00z-");
			strcat(current_all, ed);
			strcat(current_all, " @");
			strcat(current_all, ez);
			strcat(current_all, ":00z");

			temp = strsep(&taf_lines, " ");
			if (strncmp(temp, "NIL", 3) == 0) {
				asprintf(&taf_condx, "Station not active");
				nil = 1;
			}
		}
		if (strlen(taf_linzs) < 10)
			nil = 1;
// Winds / NIL=
		if (nil == 0) {
			taf_temp = tafWind(temp);

			strcpy(wind, taf_temp);
			strcpy(fc_curr_wind, "Wind:");
			strcat(fc_curr_wind, taf_temp);

// Visibility
			temp = strsep(&taf_lines, " ");
			asprintf(&visby, "n/a");
			if (temp != NULL) {
				temp_len = strlen(temp);
				if ((strncmp(temp + (temp_len -2), "SM", 2) == 0) || (temp_len == 1) ||
						(temp_len == 4)) {
					if (temp_len == 1)
						temp2 = strsep(&taf_lines, " ");

					tafVisibilty(temp, taf_temp, temp2);
					strcpy(hold, fc_curr_wind);
					strcat(hold, " <b>Visibility:</b> ");
					strcat(hold, taf_temp);

					asprintf(&visby, "%s", taf_temp);
				}
				else {
					if (taf_lines != NULL) {
						asprintf(&t_temp, "%s", taf_lines);
						temp[temp_len + 1] = '\0';
						sprintf(taf_lines, "%s %s", temp, t_temp);
					}
					else
						asprintf(&taf_lines, "%s", temp);
				}
// WX sky
				taf_condx = tafWeather(taf_lines);
				strcat(hold, "<b>Conditions:</b> ");
				strcat(hold, taf_condx);
			}
		}
		else {
			asprintf(&visby,"n/a");
			strcpy(wind,"n/a");
		}

		if (taf_line_num ==0) {
			sprintf(postsql, "INSERT INTO taf (issued, current, wind, visby, condx, rep_time, stn_call, taf_unknown_fl, taf_raw) "
					"VALUES ('%s','%s','%s','%s','%s','%s','%s',%d,'%s')",
					issued, current_all, wind, visby, taf_condx, reptime, gstn, err, taf_linzs);
		}
		else {
			sprintf(postsql, "INSERT INTO taf_forecast (rep_time, stn_call, forecast, taf_unknown_fl, taf_line_number, taf_raw) "
					"VALUES ('%s','%s','%s',%d, %d,'%s')",
					reptime, gstn, hold, err, taf_line_num, taf_linzs);
		}
		PGresult *res = PQexec(conn, postsql);
		if (PQresultStatus(res) != PGRES_COMMAND_OK) {
			if (strncmp(PQerrorMessage(conn),"ERROR:  duplicate key", 21) != 0)
				fprintf(to, "(TAF)bad sql %s \nStatus:%d\n%s\n", PQerrorMessage(conn),
						PQresultStatus(res), postsql);
		}
		PQclear(res);
	}
	else if (strcmp(taf_t, "BE") == 0) {		// BECMG

		temp = strsep(&taf_lines, " ");

		if (strncmp(temp + 4, "/", 1) != 0) {
			strcpy(hold, "Unknown: ");
			strcat(hold, taf_linzs);
			err = 1;
		}
		else {
			validDates(sd, sz, ed, ez, temp);
			strcpy(hold, "<b>Becoming:</b> ");
			strcat(hold, ed);
			strcat(hold, " @");
			strcat(hold, sz);
			strcat(hold, ":00z-");
			strcat(hold, ed);
			strcat(hold, " @");
			strcat(hold, ez);
			strcat(hold, ":00z");

			// Winds
			temp = strsep(&taf_lines, " ");
			taf_temp = tafWind(temp);
			strcat(hold, " <b>Wind:</b> ");
			strcat(hold, taf_temp);

// Visibility
			temp = strsep(&taf_lines, " ");
			if (temp == 0) {
				return;
			}
			temp_len = strlen(temp);
			if ((strncmp(temp + (temp_len - 2), "SM", 2) == 0) || (temp_len == 1) || (temp_len == 4)) {
				if (temp_len == 1)
					temp2 = strsep(&taf_lines, " ");

				tafVisibilty(temp, taf_temp, taf_lines);
				strcat(hold, " <b>Visibility:</b> ");
				strcat(hold, taf_temp);
			}
			else {
				if (!taf_lines) {
					asprintf(&taf_lines, "%s", temp);
				}
				else {
					asprintf(&t_temp, "%s", taf_lines);
					temp[temp_len + 1] = '\0';
					sprintf(taf_lines, "%s %s", temp, t_temp);
				}
			}
// WX sky
			taf_temp[0] = '\0';
			taf_condx = tafWeather(taf_lines);
			strcat(hold, " <b>Conditions:</b> ");
			strcat(hold, taf_condx);
		}

		sprintf(postsql, "INSERT INTO taf_forecast (rep_time, stn_call, forecast, taf_unknown_fl, taf_line_number, taf_raw) "
				"VALUES ('%s','%s','%s',%d, %d,'%s')",
				reptime, gstn, hold, err, taf_line_num, taf_linzs);

		PGresult *res = PQexec(conn, postsql);
		if (PQresultStatus(res) != PGRES_COMMAND_OK) {
			if (strncmp(PQerrorMessage(conn),"ERROR:  duplicate key", 21) != 0)
				fprintf(to, "(TAF)bad sql %s \nStatus:%d\n%s\n", PQerrorMessage(conn),
						PQresultStatus(res), postsql);
		}
		PQclear(res);

		temp = strsep(&taf_lines, " ");
	}
	else if (strcmp(taf_t, "FM") == 0) {		// FROM
		strncpy(sd, temp + 2, 2);
		sd[2] = '\0';
		dx = atoi(sd);
		dt = daySuffix(dx);
		sprintf(sd, "%d%s", dx, dt);
		strncpy(fsz_hr, temp + 4, 2);
		fsz_hr[2] = '\0';
		strncpy(fsz_min, temp + 6, 2);
		fsz_min[2] = '\0';
		sprintf(fsz,"%s:%s",fsz_hr,fsz_min);
		strcpy(hold, "<b>From:</b> ");
		strcat(hold, sd);
		strcat(hold, " @");
		strcat(hold, fsz);
		strcat(hold, "z");
// Winds
		temp = strsep(&taf_lines, " ");
		taf_temp = tafWind(temp);
		strcat(hold, " <b>Wind:</b> ");
		strcat(hold, taf_temp);
// Visibility
		temp = strsep(&taf_lines, " ");
		if (temp) {
			temp_len = strlen(temp);
			if ((strncmp(temp + (temp_len - 2), "SM", 2) == 0) || (temp_len == 1) || (temp_len == 4)) {
				if (temp_len == 1)
					temp2 = strsep(&taf_lines, " ");

				tafVisibilty(temp, taf_temp, temp2);
				strcat(hold, " <b>Visibility:</b> ");
				strcat(hold, taf_temp);
			}
		}
// WX sky
		taf_temp[0] = '\0';
		taf_condx = tafWeather(taf_lines);
		strcat(hold, " <b>Conditions:</b> ");
		strcat(hold, taf_condx);

		sprintf(postsql, "INSERT INTO taf_forecast (rep_time, stn_call, forecast, taf_unknown_fl, taf_line_number, taf_raw) "
				"VALUES ('%s','%s','%s',%d, %d,'%s')",
				reptime, gstn, hold, err, taf_line_num, taf_linzs);

		PGresult *res = PQexec(conn, postsql);
		if (PQresultStatus(res) != PGRES_COMMAND_OK) {
			if (strncmp(PQerrorMessage(conn),"ERROR:  duplicate key", 21) != 0)
				fprintf(to, "(TAF)bad sql %s \nStatus:%d\n%s\n", PQerrorMessage(conn),
						PQresultStatus(res), postsql);
		}
		PQclear(res);
	}
	else if (strcmp(taf_t, "TE") == 0) {	// TEMPO
		temp = strsep(&taf_lines, " ");
		validDates(sd, sz, ed, ez, temp);
		strcpy(hold, "<b>Temporary:</b> ");
		strcat(hold, sd);
		strcat(hold, " @");
		strcat(hold, sz);
		strcat(hold, ":00z");
		strcat(hold, " -");
		strcat(hold, ed);
		strcat(hold, " @");
		strcat(hold, ez);
		strcat(hold, ":00z");

		temp = strsep(&taf_lines, " ");
		if (temp != NULL) {
			temp_len = strlen(temp);

			if (strncmp(temp + (temp_len - 2), "KT", 2) == 0) {
				taf_temp = tafWind(temp);
				strcat(hold, " <b>Wind: </b>");
				strcat(hold, taf_temp);
				temp = strsep(&taf_lines, " ");
				if (temp != NULL) {
					temp_len = strlen(temp);
					sw = 1;
				}
			}
		}

		if (temp != NULL) {
			if ((strncmp(temp + (temp_len - 2), "SM", 2) == 0) || (temp_len == 1) ||
					((temp_len == 4) && isdigit(temp[0]))) {
				if (temp_len == 1)
					temp2 = strsep(&taf_lines, " ");

				tafVisibilty(temp, taf_temp, temp2);
				strcat(hold, " <b>Visibility:</b> ");
				strcat(hold, taf_temp);
				sw = 1;
			}
// WX Sky
			taf_temp[0] = '\0';

			if (taf_lines) {
				if (sw == 0) {
					sprintf (taf_cld, "%s %s", temp, taf_lines);
					taf_condx = tafWeather(taf_cld);
				}
				else
					taf_condx = tafWeather(taf_lines);
			}
			else {
				taf_condx = tafWeather(temp);
			}

			strcat(hold, " <b>Conditions:</b> ");
			strcat(hold, taf_condx);

			sprintf(postsql, "INSERT INTO taf_forecast (rep_time, stn_call, forecast, taf_unknown_fl, taf_line_number, taf_raw) "
					"VALUES ('%s','%s','%s',%d, %d,'%s')",
					reptime, gstn, hold, err, taf_line_num, taf_linzs);

			PGresult *res = PQexec(conn, postsql);
			if (PQresultStatus(res) != PGRES_COMMAND_OK) {
				if (strncmp(PQerrorMessage(conn),"ERROR:  duplicate key", 21) != 0)
					fprintf(to, "(TAF)bad sql %s \nStatus:%d\n%s\n", PQerrorMessage(conn),
							PQresultStatus(res), postsql);
			}
			PQclear(res);
		}
	}
}
