/*
 * uat_taf.c
 *
 *  Created on: Jan 14, 2021
 *      Author: TJH
 *      TAF Decode
 */

#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "uat_taf.h"
#include "uat_decode.h"

void daySuffix(int d ,char *dt)
{													// adds st, nd, rd, th to day number.
	switch (d){
		case 1: case 21: case 31: 	strcpy(dt,"st"); break;
		case 2: case 22:			strcpy(dt,"nd"); break;
		case 3: case 23: 			strcpy(dt,"rd"); break;
		default: 					strcpy(dt,"th"); break;
	}
	dt[2] = '\0';
}

void validDates(char *sd,char *sz, char *ed, char *ez, char *str)
{  													//TAF valid dates
	int d;
	char dt[3];

	sd[2] = '\0';
	ed[2] = '\0';

	strncpy(sd,str,2);
	d = atoi(sd);

	daySuffix(d ,dt);
	sprintf(sd,"%d%s",d,dt);

	strncpy(sz,str+2,2);
	sz[2] = '\0';

	strncpy(ed,str+5,2);
	d = atoi(ed);

	daySuffix(d ,dt);
	sprintf(ed,"%d%s",d,dt);

	strncpy(ez,str+7,2);
	ez[2] = '\0';
}

void tafWind(char *d,char *s,char *gs,char *temp, char *taf_wind)
{													// Decode wind/gust.
	char kt[3];
	int w_len;
	int kt_int = 0;

	gs[0] = '\0';
	w_len = strlen(temp);

	if (w_len == 7){
		strncpy(kt,temp+5,2);
		if (strcmp(kt,"KT") != 0)
			sprintf(taf_wind,"unknown(%s)",temp);
		else{
			strncpy(d,temp,3);
			d[3] ='\0';
			strncpy(s,temp+3,2);
			s[2] ='\0';
			kt_int = atoi(s);
		}
	}
	else if (w_len == 10){
		strncpy(kt,temp+8,2);
		if (strcmp(kt,"KT") != 0)
			sprintf(taf_wind,"unknown(%s)",temp);
		else{
			strncpy(d,temp,3);
			d[3] = '\0';
			strncpy(s,temp+3,2);
			s[2] = '\0';
			kt_int = atoi(s);
			strncpy(gs,temp+6,2);
			gs[2] = '\0';
		}
	}
	if (strcmp(d,"VRB") == 0)
		sprintf(taf_wind,"Wind      : Variable - Speed %dkt",kt_int);
	else
		sprintf(taf_wind,"Wind      : Dir %s deg - Speed %dkt",d,kt_int);
	if (strcmp(gs,"\0") == 0){
		if (strcmp(temp,"00000KT") == 0)
			sprintf(taf_wind,"Wind: Calm");
			else
				sprintf(taf_wind,"%s",taf_wind);
	}
	else
		sprintf(taf_wind,"%s - Gusts %skt",taf_wind,gs);
}
void tafVisibilty(char *temp, char *taf_vis)
{													// Decode visibility.
	int len_v, sm = 0;
	char vis[10];

	len_v = strlen(temp);

	if (strncmp(temp,"P",1) == 0){
		strncpy(vis,temp+1,len_v-3);
		sm=atoi(vis);
		sprintf(taf_vis,"Visibility: More than: %d statue miles",sm);
	}
	else if (strncmp(temp,"M",1) == 0){
		strncpy(vis,temp+1,len_v-3);
		sm=atoi(vis);
		sprintf(taf_vis,"Visibility: Mess than: %d statue miles",sm);
	}
	else if (strncmp(temp+(len_v-2),"SM",2) == 0){
		strncpy(vis,temp,len_v-2);

		sprintf(taf_vis,"Visibility: %s statue miles",vis);
	}
	else if (strncmp(temp,"9999",4) == 0)
		sprintf(taf_vis,"Visibility: Clear");
	else
		sprintf(taf_vis,"Visibility: %s meters",temp);
}

void tafWeather(char *taf_list,char *taf_wx)
{													// Decode weather/sky conditions.
	char *temp;
	char temp2[30];
	int units;
	float units_f;
	int l;

	temp = strsep(&taf_list," ");

	while (temp !=NULL){
		l=strlen(temp);
		if (strcmp(temp,"BR") == 0)
			sprintf(taf_wx,"%s Mist",taf_wx);
		else if (strncmp(temp,"BKN",3) == 0){
			units = atoi(temp+3);
			if (l > 6){
				strncpy(temp2,temp+6,l-6);
				temp2[2] = '\0';
					if (strcmp(temp2,"CB") == 0)
						sprintf(taf_wx,"%s Broken cumulonimbus clouds %d00ft",taf_wx,units);
					else
						sprintf(taf_wx,"%s Broken (%s) clouds %d00ft",taf_wx,temp2,units);
			}
			else
				sprintf(taf_wx,"%s Broken clouds %d00ft",taf_wx,units);
		}
		else if (strncmp(temp,"FEW",3) == 0){
			units = atoi(temp+3);
			if (l > 6){
				strncpy(temp2,temp+6,l-6);
				temp2[2]='\0';
					if (strcmp(temp2,"CB") == 0)
						sprintf(taf_wx,"%s Few cumulonimbus clouds %d00ft",taf_wx,units);
					else
						sprintf(taf_wx,"%s Few (%s) clouds %d00ft",taf_wx,temp2,units);
			}
			else
					sprintf(taf_wx,"%s Few clouds %d00ft",taf_wx,units);
		}
		else if (strncmp(temp,"OVC",3) == 0){
			units = atoi(temp+3);
			if (l > 6){
				strncpy(temp2,temp+6,l-6);
				temp2[2] = '\0';
					if (strcmp(temp2,"CB") == 0)
						sprintf(taf_wx,"%s Overcast cumulonimbus clouds %d00ft",taf_wx,units);
					else
						sprintf(taf_wx,"%s Overcast (%s) clouds %d00ft",taf_wx,temp2,units);
			}
			else
				sprintf(taf_wx,"%s Overcast %d00ft",taf_wx,units);
		}
		else if (strncmp(temp,"SCT",3) == 0){
			units = atoi(temp+3);
			if (l > 6){
				strncpy(temp2,temp+6,l-6);
				temp2[2] = '\0';
					if (strcmp(temp2,"CB") == 0)
						sprintf(taf_wx,"%s Scattered cumulonimbus clouds %d00ft",taf_wx,units);
					else
						sprintf(taf_wx,"%s Scattered (%s) clouds %d00ft",taf_wx,temp2,units);
			}
			else
				sprintf(taf_wx,"%s Scattered clouds %d00ft",taf_wx,units);
		}
		else if (strncmp(temp,"SKC",3) == 0)
			sprintf(taf_wx,"%s Sky clear",taf_wx);
		else if (strncmp(temp,"QNH",3) == 0){
			strncpy(temp2,temp+3,4);
			units_f=atof(temp2);
			sprintf(taf_wx,"%s Altimeter %2.2fHg",taf_wx,units_f/100);
		}
		else if (strcmp(temp,"AUTOMATED") == 0)
			sprintf(taf_wx,"%s AUTOMATED",taf_wx);
		else if (strcmp(temp,"BC") == 0)
			sprintf(taf_wx,"%s Patches",taf_wx);
		else if (strcmp(temp,"BL") == 0)
			sprintf(taf_wx,"%s Blowing",taf_wx);
		else if (strcmp(temp,"DR") == 0)
			sprintf(taf_wx,"%s Low drifting",taf_wx);
		else if (strcmp(temp,"DS") == 0)
			sprintf(taf_wx,"%s Duststorm",taf_wx);
		else if (strcmp(temp,"DU") == 0)
			sprintf(taf_wx,"%s Widespread dust",taf_wx);
		else if (strcmp(temp,"FC") == 0)
			sprintf(taf_wx,"%s Funnel clouds",taf_wx);
		else if (strcmp(temp,"FU") == 0)
			sprintf(taf_wx,"%s Smoke",taf_wx);
		else if (strcmp(temp,"GR") == 0)
			sprintf(taf_wx,"%s Hail",taf_wx);
		else if (strcmp(temp,"GS") == 0)
			sprintf(taf_wx,"%s Small hail and/or snow pellets",taf_wx);
		else if (strcmp(temp,"IC") == 0)
			sprintf(taf_wx,"%s Ice crystals",taf_wx);
		else if (strcmp(temp,"MI	") == 0)
			sprintf(taf_wx,"%s Shallow",taf_wx);
		else if (strcmp(temp,"PL") == 0)
			sprintf(taf_wx,"%s Ice pellets",taf_wx);
		else if (strcmp(temp,"PO") == 0)
			sprintf(taf_wx,"%s Dust/sand whirls",taf_wx);
		else if (strcmp(temp,"PR") == 0)
			sprintf(taf_wx,"%s Partial",taf_wx);
		else if (strcmp(temp,"SA") == 0)
			sprintf(taf_wx,"%s Sand",taf_wx);
		else if (strcmp(temp,"SG") == 0)
			sprintf(taf_wx,"%s Snow grains",taf_wx);
		else if (strcmp(temp,"SQ") == 0)
			sprintf(taf_wx,"%s Squall(s)",taf_wx);
		else if (strcmp(temp,"SS") == 0)
			sprintf(taf_wx,"%s Sandstorm",taf_wx);
		else if (strcmp(temp,"VA") == 0)
			sprintf(taf_wx,"%s Volcanic ash",taf_wx);
		else if (strcmp(temp,"UP") == 0)
			sprintf(taf_wx,"%s Unknown precipitation",taf_wx);
		else if (strcmp(temp,"-DZ") == 0)
			sprintf(taf_wx,"%s Light drizzle",taf_wx);
		else if (strcmp(temp,"DZ") == 0)
			sprintf(taf_wx,"%s Drizzle",taf_wx);
		else if (strcmp(temp,"FG") == 0)
			sprintf(taf_wx,"%s Fog",taf_wx);
		else if (strcmp(temp,"FZFG") == 0)
			sprintf(taf_wx,"%s Freezing fog",taf_wx);
		else if (strcmp(temp,"-FZRA") == 0)
			sprintf(taf_wx,"%s Light freezing rain",taf_wx);
		else if (strcmp(temp,"HZ") == 0)
			sprintf(taf_wx,"%s Haze",taf_wx);
		else if (strcmp(temp,"NSW") == 0)
			sprintf(taf_wx,"%s No significant weather",taf_wx);
		else if (strcmp(temp,"-RA") == 0)
			sprintf(taf_wx,"%s Light rain",taf_wx);
		else if (strcmp(temp,"RA") == 0)
			sprintf(taf_wx,"%s Rain",taf_wx);
		else if (strcmp(temp,"-RADZ") == 0)
			sprintf(taf_wx,"%s Light rain\\drizzle",taf_wx);
		else if (strcmp(temp,"-RASN") == 0)
			sprintf(taf_wx,"%s Light rain\\snow",taf_wx);
		else if (strcmp(temp,"-SHRA") == 0)
			sprintf(taf_wx,"%s Light rain showers",taf_wx);
		else if (strcmp(temp,"SHRA") == 0)
			sprintf(taf_wx,"%s Rain showers",taf_wx);
		else if (strcmp(temp,"-SN") == 0)
			sprintf(taf_wx,"%s Light snow",taf_wx);
		else if (strcmp(temp,"SN") == 0)
			sprintf(taf_wx,"%s Snow",taf_wx);
		else if (strcmp(temp,"-SNRA") == 0)
			sprintf(taf_wx,"%s Light snow\rain",taf_wx);
		else if (strcmp(temp,"-SHSN") == 0)
			sprintf(taf_wx,"%s Light snow shower",taf_wx);
		else if (strcmp(temp,"TSRA") == 0)
			sprintf(taf_wx,"%s Thunderstorm and rain",taf_wx);
		else if (strncmp(temp,"6",1) == 0){
			units=strlen(temp);
			if (units == 6){
				int ice1,ice2,ice3;
				char ice_int[50];

				ice_int[0] = '\0';
				strncpy(temp2,temp+1,1);
				temp2[1] = '\0';
				ice1 = atoi(temp2);

				switch(ice1){
					case 0: strcpy(ice_int,"Trace Icing"); break;
					case 1: strcpy(ice_int,"Light Mixed Icing"); break;
					case 2: strcpy(ice_int,"Light Rime Icing In Cloud"); break;
					case 3: strcpy(ice_int,"Light Clear Icing In Precipitation"); break;
					case 4: strcpy(ice_int,"Moderate Mixed Icing"); break;
					case 5: strcpy(ice_int,"Moderate Rime Icing In Cloud"); break;
					case 6: strcpy(ice_int,"Moderate Clear Icing In Precipitation"); break;
					case 7: strcpy(ice_int,"Severe Mixed Icing"); break;
					case 8: strcpy(ice_int,"Severe Rime Icing In Cloud"); break;
					case 9: strcpy(ice_int,"Severe Clear Icing In Precipitation"); break;
				}
				strncpy(temp2,temp+2,3);
				temp2[3] = '\0';
				ice2 = (atoi(temp2)*100);
				strncpy(temp2,temp+5,1);
				temp2[1] = '\0';
				ice3 = (atoi(temp2)*1000);

				sprintf(taf_wx,"%s %s %dft-%dft",taf_wx,ice_int,ice2,ice2+ice3);
			}
			else
				sprintf(taf_wx,"%s (unknown %s) ",taf_wx,temp);
		}
		else if (strcmp(temp,"VCSH") == 0)
			sprintf(taf_wx,"%s Showers in the vicinity",taf_wx);
		else if (strcmp(temp,"VCTS") == 0)
			sprintf(taf_wx,"%s Thunderstorm in vicinity",taf_wx);
		else if (strncmp(temp,"VV",2) == 0){
			int v;
			strcpy(temp2,temp+2);
			v = (atoi(temp2) * 100);
			sprintf(taf_wx,"%s Verical visibilty %dft",taf_wx,v);
		}
		else if (strncmp(temp,"TNM",3) == 0) {
			char dt[3];
			float fahr;

			strncpy(temp2,temp+3,2);
			temp2[2] = '\0';
			units = (atoi(temp2)* -1);
			fahr = (units * 9/5 ) +32;
			strncpy(temp2,temp+6,2);
			int d = atoi(temp2);

			daySuffix(d ,dt);

			strncpy(temp2,temp+8,2);
			temp2[2] = '\0';

			sprintf(taf_wx,"%s Temp min %.1ff on %d%s at %s:00z",taf_wx,fahr,d,dt,temp2);
		}
		else if ((strncmp(temp,"TX",2) == 0) || (strncmp(temp,"TN",2) == 0)) {
			char dt[3];
			float fahr;

			strncpy(temp2,temp+2,2);
			temp2[2] = '\0';
			units = atoi(temp2);
			fahr = (units * 9/5 ) +32;
			strncpy(temp2,temp+5,2);
			int d = atoi(temp2);

			daySuffix(d ,dt);

			strncpy(temp2,temp+7,2);
			temp2[2] = '\0';

			if (strncmp(temp,"TX",2) == 0)
				sprintf(taf_wx,"%s Max Temp %.1ff on %d%s %s:00z",taf_wx,fahr,d,dt,temp2);
			else
				sprintf(taf_wx,"%s Min Temp %.1ff on %d%s %s:00z",taf_wx,fahr,d,dt,temp2);
		}
		else if ((strncmp(temp,"T",1) == 0) &&  (strncmp(temp+3,"/",1) == 0) ) {
			char dt[3];
			float fahr;

			strncpy(temp2,temp+1,2);
			temp2[2] = '\0';
			units = atoi(temp2);
			fahr = (units * 9/5 ) +32;
			strncpy(temp2,temp+4,2);
			int d = atoi(temp2);

			daySuffix(d ,dt);

			strncpy(temp2,temp+6,2);
			temp2[2] = '\0';

			sprintf(taf_wx,"%s Temperature %.1ff on %d%s %s:00z",taf_wx,fahr,d,dt,temp2);
		}
		else if (strncmp(temp,"WS",2) == 0)	{
			int h,len_t;
			char d[5],s[5],gs[5],taf_wind[50];
			strncpy(temp2,temp+2,3);
			h=(atoi(temp2)*100);
			len_t = strlen(temp);
			strncpy(temp2,temp+6,len_t - 6);
			tafWind(d,s,gs,temp2,taf_wind);

			sprintf(taf_wx,"%s Wind shear at %dft %s",taf_wx,h,taf_wind);
		}
		else if (strncmp(temp,"PROB",4) == 0) {
			char sd[5],sz[3],ed[5],ez[3];
			char perc[3];
			strncpy(perc,temp+4,2);
			perc[2] = '\0';
			temp = strsep(&taf_list," ");
			validDates(sd,sz,ed,ez,temp);

			temp = strsep(&taf_list," ");
			tafVisibilty(temp,temp2);
			sprintf(taf_wx,"%s %s%% Probability %s %s:00z - %s %s:00z %s",taf_wx,perc,sd,sz,ed,ez,temp2);
		}
		else if (strncmp(temp,"5",1) == 0){
			units=strlen(temp);
			if (units == 6){
				int turb1,turb2,turb3;
				char turb_int[50];

				turb_int[0] = '\0';
				strncpy(temp2,temp+1,1);
				temp2[1] = '\0';
				if (temp2[0] == 'X')
					strcpy(turb_int,"Extreme turbulence");
				else {
					turb1 = atoi(temp2);

					switch(turb1){
						case 0: strcpy(turb_int,"None"); break;
						case 1: strcpy(turb_int,"Light turbulence"); break;
						case 2: strcpy(turb_int,"Moderate turbulence in clear air, occasional"); break;
						case 3: strcpy(turb_int,"Moderate turbulence in clear air, frequent"); break;
						case 4: strcpy(turb_int,"Moderate turbulence in cloud, occasional"); break;
						case 5: strcpy(turb_int,"Moderate turbulence in cloud, frequent"); break;
						case 6: strcpy(turb_int,"Severe turbulence in clear air, occasional"); break;
						case 7: strcpy(turb_int,"Severe turbulence in clear air, frequent"); break;
						case 8: strcpy(turb_int,"Severe turbulence in cloud, occasional"); break;
						case 9: strcpy(turb_int,"Severe turbulence in cloud, frequent"); break;
					}
				}
				strncpy(temp2,temp+2,3);
				temp2[3] = '\0';
				turb2 = (atoi(temp2)*100);
				strncpy(temp2,temp+5,1);
				temp2[1] = '\0';
				turb3 = (atoi(temp2)*1000);

				sprintf(taf_wx,"%s %s %dft-%dft",taf_wx,turb_int,turb2,turb2+turb3);
			}
			else
				sprintf(taf_wx,"%s (unknown %s)",taf_wx,temp);
		}
		else
			sprintf(taf_wx,"%s Unknown (%s)",taf_wx,temp);

		temp = strsep(&taf_list," ");
		}
}

void taf_decode(char *taf_lines)
{													// TAF Decode

	char sd[10],sz[3],ed[5],ez[3];
	char dt[3];
	char fsz[5];
	char taf_t[5];
	char d[4], s[4],gs[4];
	char *temp;
	char taf_temp[200];
	char taf_cld[50];
	int dx;
	int temp_len;
	int sw = 0;

	// Valid dates
	strncpy(taf_t,taf_lines,2);
	taf_t[2] = '\0';

	if(isdigit(taf_t[0])) {      		 // 1st line?
		temp = strsep(&taf_lines," ");
		validDates(sd,sz,ed,ez,temp);
		fprintf(filetaf,"Current  : %s at %s:00z ",sd,sz);
		fprintf(filetaf,"- %s at %s:00z\n",ed,ez);

	// winds
		temp = strsep(&taf_lines," ");
		tafWind(d,s,gs,temp,taf_temp);
		fprintf(filetaf,"  %s\n",taf_temp);

	// visibility
		temp = strsep(&taf_lines," ");
		tafVisibilty(temp,taf_temp);
		fprintf(filetaf,"  %s\n",taf_temp);

	// WX sky
		taf_temp[0] = '\0';
		tafWeather(taf_lines,taf_temp);
		fprintf(filetaf,"  Conditions:%s\n\n",taf_temp);
	}
	else if (strcmp(taf_t,"BE") == 0){						// BECMG
		temp = strsep(&taf_lines," ");
		temp = strsep(&taf_lines," ");
		validDates(sd,sz,ed,ez,temp);
		fprintf(filetaf,"Becoming : %s at %s:00z ",sd,sz);
		fprintf(filetaf,"- %s at %s:00z\n",ed,ez);

	// winds
		temp = strsep(&taf_lines," ");
		tafWind(d,s,gs,temp,taf_temp);
		fprintf(filetaf,"  %s\n",taf_temp);

	//visibility
		temp = strsep(&taf_lines," ");
		tafVisibilty(temp,taf_temp);
		fprintf(filetaf,"  %s\n",taf_temp);

	// WX sky
		taf_temp[0] = '\0';
		tafWeather(taf_lines,taf_temp);
		fprintf(filetaf,"  Conditions:%s\n\n",taf_temp);
		temp = strsep(&taf_lines," ");
	}
	else if (strcmp(taf_t,"FM") == 0){						// FROM
		temp = strsep(&taf_lines," ");
		strncpy(sd,temp+2,2);
		sd[2] = '\0';
		dx=atoi(sd);
		daySuffix(dx ,dt);
		sprintf(sd,"%d%s",dx,dt);
		strncpy(fsz,temp+4,4);
		fsz[4] = '\0';
		fprintf(filetaf,"From: %s at %sz\n",sd,fsz);

	// winds
		temp = strsep(&taf_lines," ");
		tafWind(d,s,gs,temp,taf_temp);
		fprintf(filetaf,"  %s\n",taf_temp);

	// visibility
		temp = strsep(&taf_lines," ");
		tafVisibilty(temp,taf_temp);
		fprintf(filetaf,"  %s\n",taf_temp);

	// WX sky
		taf_temp[0] = '\0';
		tafWeather(taf_lines,taf_temp);
		fprintf(filetaf,"  Conditions:%s\n\n",taf_temp);
	}
	else if (strcmp(taf_t,"TE") == 0){						// TEMPO
		temp = strsep(&taf_lines," ");
		validDates(sd,sz,ed,ez,taf_lines);
		fprintf(filetaf,"Temporary: %s at %s:00z ",sd,sz);
		fprintf(filetaf,"- %s at %s:00z\n",ed,ez);

		temp = strsep(&taf_lines," ");
		temp = strsep(&taf_lines," ");

		temp_len = strlen(temp);

		if (strncmp(temp+(temp_len-2),"KT",2) == 0){
			tafWind(d,s,gs,temp,taf_temp);
			fprintf(filetaf,"  %s\n",taf_temp);
			temp = strsep(&taf_lines," ");
			temp_len=strlen(temp);
			sw = 1;
		}
		if (strncmp(temp+(temp_len-2),"SM",2) == 0){
			tafVisibilty(temp,taf_temp);
			fprintf(filetaf,"  %s\n",taf_temp);
			sw = 1;
		}
		else if (temp_len == 4){
			int vis_met = 0;
			for (int k = 0; k < 4; ++k) {
				if (isdigit(temp[k]))
					vis_met++;
			}
			if (vis_met == 4){
				tafVisibilty(temp,taf_temp);
				fprintf(filetaf,"  %s\n",taf_temp);
				sw = 1;
			}
		}

	// WX Sky
		taf_temp[0] = '\0';

		if (taf_lines){
			if (sw == 0){
				sprintf (taf_cld,"%s %s",temp,taf_lines);
				tafWeather(taf_cld,taf_temp);
			}
			else{
				tafWeather(taf_lines,taf_temp);
			}
		}
		else {
			tafWeather(temp,taf_temp);
		}
		fprintf(filetaf,"  Conditions:%s\n\n",taf_temp);
	}
	else if (strcmp(taf_t,"QN") == 0){			// Military
		taf_temp[0] = '\0';
		fprintf(filetaf,"Other    : %s\n",taf_lines);
		tafWeather(taf_lines,taf_temp);
				fprintf(filetaf,"  Conditions:%s\n\n",taf_temp);
	}
	else if (strcmp(taf_t,"VR") == 0){			// Military?
		fprintf(filetaf,"Other    : %s\n",taf_lines);
		temp = strsep(&taf_lines," ");

		tafWind(d,s,gs,temp,taf_temp);
		fprintf(filetaf,"  %s\n",taf_temp);

		temp = strsep(&taf_lines," ");

		tafVisibilty(temp,taf_temp);
		fprintf(filetaf,"  %s\n",taf_temp);
		taf_temp[0] = '\0';

		tafWeather(taf_lines,taf_temp);
		fprintf(filetaf,"  Conditions:%s\n\n",taf_temp);
	}
	else
		fprintf(filetaf,"Not found %s\n",taf_lines);
}
