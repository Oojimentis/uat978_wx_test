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
 
## Example config file (config.txt)

````
# postgresql stuff
pg_user = your_postgres_username
pg_pwd = postgres_password
pg_db = database_name
file_path = /path_to_node_public_directory (not used)

# process NEXRAD - For testing, run time is quicker with NEXRAD off (N)
print_nexrad = N

# Clean out database at the start of each run
reset_database = Y
````
## Run

    > rtl_sdr -g 0 -d 0 -f 978000000 -s 2083334 - | ./dump978 | tee  >(./uat2text > log.txt)

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

METAR (Metrological Aviation Report)

![METAR](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/metar_screen.png)

Current Report List:
![Frame info](https://github.com/Oojimentis/uat978_wx_test/blob/master/uat978_wx_text/docs/info_frame_screen.png)

Service Status:

![service](https://user-images.githubusercontent.com/60933475/126045501-eb4d9b2d-ae3d-4a7c-9fca-13085905abbe.png)



## Database

Examples of data collected: (See https://github.com/Oojimentis/map-978/blob/main/maps/schema.sql for full schema)

METAR:
![metar](https://user-images.githubusercontent.com/60933475/126044880-57662300-3e44-4dfb-af82-124967a7f17f.png)

AIRMET, CWA, G-AIRMET, NOTAM and SIGMET graphical data:
![graphics](https://user-images.githubusercontent.com/60933475/126044944-73f0cbfb-a3dd-47a6-9925-cdc399e92a7a.png)

Extended Range Circular Prism data:
![circles](https://user-images.githubusercontent.com/60933475/126045020-feeec8bd-7bc5-4081-894a-dffe0d8508ad.png)

AIRMET, NOTAM and SIGMET Text Reports:
![sigairmet](https://user-images.githubusercontent.com/60933475/126045049-d1c285f9-b551-4a1c-8951-2c7c847d4ff5.png)

TAF:
![taf](https://user-images.githubusercontent.com/60933475/126045061-b5eeb62d-38ad-45c9-bd3d-7d32f7048699.png)

TAF Forecast:
![taf_forecast](https://user-images.githubusercontent.com/60933475/126045070-74f4a6ae-6fdc-4b30-812f-efc7ccacec7e.png)

PIREP:
![pirep](https://user-images.githubusercontent.com/60933475/126045085-22640df3-df07-4994-8e0d-223d7ae0d1b3.png)

SUA:
![sua](https://user-images.githubusercontent.com/60933475/126045101-7864526d-b46f-4a8a-b284-ff34cb532b65.png)

Winds aloft
![winds](https://user-images.githubusercontent.com/60933475/126045130-e71ef82b-3e6c-48ee-bfcf-b490c36b6f7c.png)

NEXRAD:
![nexrad](https://user-images.githubusercontent.com/60933475/126045158-a9143d87-7eb4-44ce-9742-58d90166f6e9.png)
