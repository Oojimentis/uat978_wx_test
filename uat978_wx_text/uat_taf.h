/*
 * uat_taf.h
 *
 *  Created on: Jan 14, 2020
 *      Author: TH
 */

#ifndef UAT_TAF_H_INCLUDED
#define UAT_TAF_H_INCLUDED

#define BC			199
#define BL			208
#define BR			214
#define DR			234
#define DS			235
#define DU			237
#define DZ			242
#define FC			239
#define FG			243
#define FU			257
#define GRHAIL		264
#define GS			265
#define HZ			282
#define IC			269
#define MI			315
#define PL			348
#define PO			351
#define PR			354
#define RA			357
#define SA			367
#define SG			373
#define SN			380
#define SQ			383
#define SS			385
#define TS   		395
#define VA			397
#define UP			402

#define HRA			18446744073709551473u
#define HSN			18446744073709551496u
#define LDZ			18446744073709551558u
#define LPL			48
#define LRA			57
#define LSN			80
#define NSW			3389

#define BCFG		20143
#define BLSN		21180
#define DRSN		23780
#define FZFG		26443
#define FZRA		26557
#define MIFG   		31743
#define PLSN		35180
#define RAPL		36048
#define RASN		36080
#define SHRA		37757
#define SHRN		37770
#define SHSN		37780
#define SNPL		38348
#define TSRA		39857
#define TSSN		39880
#define VCFG		40143
#define VCSH		40274
#define VCTS		40295

#define HSHRA		18446744073709539373u
#define HSHSN		18446744073709539396u
#define HTSRA		18446744073709541473u
#define LDZSN		18446744073709546196u
#define LFRZA		18446744073709547453u
#define LFZDZ		18446744073709548058u
#define LFZRA		18446744073709548173u
#define LPLRA		5157
#define LPLSN		5180
#define LRADZ		5942
#define LRAPL		6048
#define LRASN		6080
#define LSHRA		7757
#define LSHSN		7780
#define LSNDZ		8242
#define LSNPL		8348
#define LSNRA		8357
#define LTSRA		9857

#define FZRAPL		2656048
#define FZRASN		2656080
#define RASNPL		3608348
#define SHRADZ		3775942
#define SHRASN		3776080
#define TSRAGR		3985964

#define HTSRAGR		18446744073708537580u
#define LFZDZPL		18446744073709196164u
#define LFZDZSN		18446744073709196196u
#define LFZRADZ		18446744073709207558u
#define LFZRAPL		18446744073709207664u
#define LFZRASN		18446744073709207696u
#define LRASNPL		608348
#define LSHRADZ		775942
#define LSHRAPL		776048
#define LSHRASN		776080
#define LSHSNRA		778357
#define LTSRAGR   	985964

#define FZRASNPL	265608348

#define AUTOMATED	2109410830
#define LFZDZSNPL	18446744073674009964u
#define LFZRAPLSN	18446744073675156796u
#define LFZRASNPL	18446744073675159964u
#define LSHRAPLSN	77605180

void taf_decode(char *taf_lines, char *issued, char *fsz, char *gstn, int j);

char* daySuffix(int d);

void validDates(char *sd, char *sz, char *ed, char *ez, char *str);

char* tafWind(char *temp);

void tafVisibilty(char *temp, char *taf_vis, char *taf_lines);

char* tafWeather(char *taf_list);


#endif /* UAT_TAF_H_ */
