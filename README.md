# uat978_wx_test

This is a clone of DUMP978 (https://github.com/mutability/dump978) written by Oliver Jowett, that I have expanded upon purely for my own amusement. It uses the METAR decoder library libmetar.a from https://github.com/flightaware/mdsplib (Eric McCarthy eric@limulus.net) and borrows from Christopher Young's Stratux project (https://github.com/cyoung/stratux Copyright (c) 2015-2016 Christopher Young).

My programming skills are somewhat limited, and this is my first Github project, so be nice! I am just doing this for my own amusement, and the hope that someone more experienced can take it further.

## Added
* WX station and airport lookup
* METAR decode (libmetar.a)
* Attempt at handling segmented NOTAM-TFR reports
* Output files for each FIS-B product. (overkill...)
* Decoding of Cloud Tops, Icing, Lightening, and Turbulence radar graphic reports like nexrad radar in DUMP978
* SUA and PIREP decode
* AIRMET, NOTAM, SIGMET, TAF, and Winds aloft text report.
* AIRMET, G-AIRMET, NOTAM, SIGMET graphic reports - Decode parameters and coordinates. (Would love to be able to plot on map)
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

gnome-terminal --tab --title="dump978 - rtl:$rtl" -- bash -ic "rtl_sdr -g $gain -d $rtl -f 978000000 -s 2083334 - | ./dump978 | tee  >(./uat2text > slog.txt) | ./uat2esnt | nc -q1 localhost 30001" 

gnome-terminal --tab --title="slog.txt" -- bash -ic "multitail -cS dump978 slog.txt"
gnome-terminal --tab --title="metar"    -- bash -ic "multitail -cS dump978 metar.out"
gnome-terminal --tab --title="sua"      -- bash -ic "multitail -cS dump978 sua.out"
gnome-terminal --tab --title="notam"    -- bash -ic "multitail -cS dump978 notam.out"
gnome-terminal --tab --title="airmet"   -- bash -ic "multitail -cS dump978 airmet.out"
gnome-terminal --tab --title="sigmet"   -- bash -ic "multitail -cS dump978 sigmet.out"
gnome-terminal --tab --title="g-airmet" -- bash -ic "multitail -cS dump978 gairmet.out"
gnome-terminal --tab --title="pirep"    -- bash -ic "multitail -cS dump978 pirep.out"
````



## Screenshots

Aircraft

![Aircraft](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/aircraft_screen.png)

AIRMET (Airmen's meteorological information)

![AIRMET graphic](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/airmet_graphic_screen.png)
![AIRMET text](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/airmet_text_screen.png)

G-AIRMET (Graphical AIRMET)

![G-AIRMET text](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/g-airmet_screen.png)
![G-AIRMET](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/g-airmet2_screen.png)

Current report list and service status

![Frame info](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/info_frame_screen.png)
![Service status](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/service_status_screen.png)

METAR (Metrological Aviation Report)

![METAR](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/metar_screen.png)

NOTAM (Notices to Airmen)

![NOTAM graphic](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/notam_graphic_screen.png)
![NOTAM text](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/notam_text_screen.png)

PIREP (Pilot reports)

![PIREP](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/pirep_screen.png)

SIGMET (Significant Metrological Information)

![SIGMET](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/sigmet_screen.png)

SUA (Special use airspace)

![SUA](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/sua_screen.png)

TAF (Terminal Aerodrome Forecast)

![TAF](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/taf_screen.png)

Winds aloft

![WINDS](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/winds_screen.png)
