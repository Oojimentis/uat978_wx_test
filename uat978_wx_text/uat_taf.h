/*
 * uat_taf.h
 *
 *  Created on: Jan 14, 2020
 *      Author: TJH
 */

#ifndef UAT_TAF_H_INCLUDED
#define UAT_TAF_H_INCLUDED

void taf_decode(char *taf_lines,char *issued, char *fsz,char *gstn);

void daySuffix(int d ,char *dt);

void validDates(char *sd,char *sz, char *ed, char *ez, char *str);

void tafWind(char *d,char *s,char *gs,char *temp, char *taf_wind);

void tafVisibilty(char *temp, char *taf_vis);

void tafWeather(char *taf_list,char *taf_wx);


#endif /* UAT_TAF_H_ */
