/*
 * uat_taf.h
 *
 *  Created on: Jan 14, 2020
 *      Author: TJH
 */

#ifndef UAT_TAF_H_INCLUDED
#define UAT_TAF_H_INCLUDED

#define BC		199
#define BL		208
#define BLSN	21180
#define DR		234
#define DS		235
#define DU		237
#define FC		239
#define FU		257
#define GRHAIL	264
#define GS		265
#define IC		269
#define MI		315
#define PL		348
#define PO		351
#define PR		354
#define SA		367
#define SG		373
#define SQ		383
#define SS		385
#define VA		397
#define UP		402
#define LDZ		18446744073709551558u
#define DZ		242
#define FG		243
#define FZFG	26443
#define LFZRA	18446744073709548173u
#define HZ		282
#define NSW		3389
#define LRA		57
#define RA		357
#define LRADZ	5942
#define LRASN	6080
#define LSHRA	7757
#define SHRA	37757
#define LSN		80
#define SN		380
#define LSNRA	8357
#define LSHSN	7780
#define LSNDZ	8242
#define LFZDZSN	18446744073709196196u
#define LFZRADZ	18446744073709207558u
#define LFZDZ	18446744073709548058u
#define LSNPL	8348
#define LSHRASN	776080
#define LSHRAPL	776048
#define LSHRAPLSN	77605180
#define TSRA	39857
#define FZRAPL	2656048
#define LFZRASN	18446744073709207696u
#define HSN		18446744073709551496u
#define HRA		18446744073709551473u
#define LPL		48
#define LFZRAPLSN	18446744073675156796u
#define BR		214
#define AUTOMATED	2109410830
#define VCSH	40274
#define VCTS	40295
#define LFZRASNPL	18446744073675159964u
#define LPLSN	5180
#define RASN	36080
#define LFZRAPL	18446744073709207664u
#define LRAPL	6048
#define SHSN	37780
#define DRSN	23780
#define SNPL	38348
#define BCFG	20143
#define FZRA   	26557
#define RASNPL	3608348
#define FZRASNPL   265608348
#define LPLRA   5157


void taf_decode(char *taf_lines, char *issued, char *fsz, char *gstn);

char* daySuffix(int d);

void validDates(char *sd, char *sz, char *ed, char *ez, char *str);

char* tafWind(char *temp);

void tafVisibilty(char *temp, char *taf_vis, char *taf_lines);

char* tafWeather(char *taf_list);


#endif /* UAT_TAF_H_ */
