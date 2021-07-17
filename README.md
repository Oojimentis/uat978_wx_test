# uat978_wx_test

This is a clone of DUMP978 (https://github.com/mutability/dump978) written by Oliver Jowett, that I have expanded upon purely for my own amusement. This version decodes all text, TWGO (Text with Graphical Overlay) and Global Block (NEXRAD) FIS-B reports which are then stored in a PostgreSQL database. This data can be mapped using MAP-978 (https://github.com/Oojimentis/map-978).

Uat978_wx_text uses the METAR decoder library libmetar.a from https://github.com/flightaware/mdsplib (Eric McCarthy eric@limulus.net) and borrows from Christopher Young's Stratux project (https://github.com/cyoung/stratux Copyright (c) 2015-2016 Christopher Young).

This is my first Github project, so be nice! I am just doing this for my own amusement, and the hope that someone more experienced can take it further.


## Added
* TAF forecast lines added to database. (7/12/21)
* Segmented G-AIRMET reports. (6/12/21)
* Segmented graphical NOTAM reports. (06/05/21)
* Segemented reports added to database. (06/05/21)
* Removed most of the old output text files, as data is now being put into database. (01/01/21)
* NEXRAD and other radar data captured in database. (12/27/20)
* Converted sqlite database to PostgreSQL. (01/01/21)
* WX station and airport lookup via database.
* METAR decode (libmetar.a)
* TAF decoded. (01/01/21)
* Decoding of Cloud Tops, Icing, Lightening, and Turbulence radar graphic reports like nexrad radar in DUMP978
* SUA and PIREP decode
* AIRMET, NOTAM, SIGMET, TAF, CWA and Winds aloft text reports.
* AIRMET, G-AIRMET, NOTAM, SIGMET and CWA graphic reports - Decode parameters and coordinates.
* Use Multitail (https://vanheusden.com/multitail) to colorize log files.


## Building

    > make
    
## Run

    > rtl_sdr -g 0 -d 0 -f 978000000 -s 2083334 - | ./dump978 | tee  >(./uat2text > slog.txt)

I use a script to launch DUMP978, output position data to DUMP1090, and view some of the output files in tabs using Multitail.

````
#!/bin/bash

read -p "rtl [0]: " rtl
rtl=${rtl:-0}
read -p "gain [0]: " gain
gain=${gain:-0}

cd /your-directory

gnome-terminal --tab --title="dump978 - rtl:$rtl" -- bash -ic "rtl_sdr -g $gain -d $rtl -f 978000000 -s 2083334 - | ./dump978 | tee  >(./uat2text > log.txt) | ./uat2esnt | nc -q1 localhost 30001" 

gnome-terminal --tab --title="log.txt" -- bash -ic "multitail -cS dump978 log.txt"
gnome-terminal --tab --title="metar"   -- bash -ic "multitail -cS dump978 metar.out"

````


## Screenshots

Aircraft

![Aircraft](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/aircraft_screen.png)

AIRMET (Airmen's meteorological information)
<br>(No longer produced - data added to database)
![AIRMET graphic](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/airmet_graphic_screen.png)
![AIRMET text](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/airmet_text_screen.png)

G-AIRMET (Graphical AIRMET)
<br>(No longer produced - data added to database)
![G-AIRMET text](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/g-airmet_screen.png)
![G-AIRMET](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/g-airmet2_screen.png)

Current report list and 
service status

![Frame info](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/info_frame_screen.png)

![Service status](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/service_status_screen.png)

METAR (Metrological Aviation Report)

![METAR](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/metar_screen.png)

NOTAM (Notices to Airmen)
<br>(No longer produced - data added to database)
![NOTAM graphic](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/notam_graphic_screen.png)
![NOTAM text](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/notam_text_screen.png)

PIREP (Pilot reports)
<br>(No longer produced - data added to database)
![PIREP](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/pirep_screen.png)

SIGMET (Significant Metrological Information)
<br>(No longer produced - data added to database)
![SIGMET](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/sigmet_screen.png)

SUA (Special use airspace)
<br>(No longer produced - data added to database)
![SUA](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/sua_screen.png)

TAF (Terminal Aerodrome Forecast)
<br>(NO longer produced - data sent to database)
![TAF](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/taf_screen.png)

Winds aloft

![WINDS](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/winds_screen.png)
