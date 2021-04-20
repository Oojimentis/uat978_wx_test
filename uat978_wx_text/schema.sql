--
-- PostgreSQL database dump
--

-- Dumped from database version 11.11 (Ubuntu 11.11-1.pgdg18.04+1)
-- Dumped by pg_dump version 11.11 (Ubuntu 11.11-1.pgdg18.04+1)

-- Started on 2021-04-20 14:01:19 EDT

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- TOC entry 10 (class 2615 OID 16394)
-- Name: postgis; Type: SCHEMA; Schema: -; Owner: postgres
--

CREATE SCHEMA postgis;


ALTER SCHEMA postgis OWNER TO postgres;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- TOC entry 200 (class 1259 OID 17416)
-- Name: airspace_ob_ele; Type: TABLE; Schema: postgis; Owner: postgres
--

CREATE TABLE postgis.airspace_ob_ele (
    airspace_ob_ele_id integer NOT NULL,
    airspace_ob_ele_desc text NOT NULL
);


ALTER TABLE postgis.airspace_ob_ele OWNER TO postgres;

--
-- TOC entry 207 (class 1259 OID 17495)
-- Name: circles; Type: TABLE; Schema: postgis; Owner: postgres
--

CREATE TABLE postgis.circles (
    alt_top integer,
    alt_bot integer,
    alpha integer,
    prod_id integer NOT NULL,
    rec_count integer NOT NULL,
    rep_num integer NOT NULL,
    rep_year integer,
    start_date character varying,
    stop_date character varying,
    geo_opt integer NOT NULL,
    r_lng integer,
    r_lat integer,
    bot public.geometry NOT NULL,
    top public.geometry NOT NULL
);


ALTER TABLE postgis.circles OWNER TO postgres;

--
-- TOC entry 201 (class 1259 OID 17422)
-- Name: fisb_products; Type: TABLE; Schema: postgis; Owner: postgres
--

CREATE TABLE postgis.fisb_products (
    fisb_product_id integer NOT NULL,
    fisb_product_desc text NOT NULL
);


ALTER TABLE postgis.fisb_products OWNER TO postgres;

--
-- TOC entry 225 (class 1259 OID 21049)
-- Name: graphics; Type: TABLE; Schema: postgis; Owner: postgres
--

CREATE TABLE postgis.graphics (
    coords public.geometry,
    alt integer NOT NULL,
    ob_ele character varying,
    rep_num integer NOT NULL,
    prod_id integer NOT NULL,
    start_date character varying,
    stop_date character varying,
    geo_overlay_opt integer NOT NULL,
    stn_call character varying(5)
);


ALTER TABLE postgis.graphics OWNER TO postgres;

--
-- TOC entry 202 (class 1259 OID 17434)
-- Name: metar; Type: TABLE; Schema: postgis; Owner: postgres
--

CREATE TABLE postgis.metar (
    stn_call character(5) NOT NULL,
    ob_date character(15) NOT NULL,
    winddir character varying(5),
    temp character varying(5),
    dewp character varying(5),
    visby character varying(5),
    windsp character(5),
    altimeter character(10)
);


ALTER TABLE postgis.metar OWNER TO postgres;

--
-- TOC entry 233 (class 1259 OID 30864)
-- Name: nexrad; Type: TABLE; Schema: postgis; Owner: postgres
--

CREATE TABLE postgis.nexrad (
    intensity integer,
    coords public.geometry NOT NULL,
    altitude integer NOT NULL,
    prod_id integer NOT NULL,
    cc integer NOT NULL,
    block_num integer NOT NULL,
    maptime character varying(6) NOT NULL,
    ice_sld integer,
    ice_prob integer
);


ALTER TABLE postgis.nexrad OWNER TO postgres;

--
-- TOC entry 203 (class 1259 OID 17446)
-- Name: nexrad_counter_seq; Type: SEQUENCE; Schema: postgis; Owner: postgres
--

CREATE SEQUENCE postgis.nexrad_counter_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE postgis.nexrad_counter_seq OWNER TO postgres;

--
-- TOC entry 204 (class 1259 OID 17454)
-- Name: overlay_geo_opt; Type: TABLE; Schema: postgis; Owner: postgres
--

CREATE TABLE postgis.overlay_geo_opt (
    overlay_geo_opt_id integer NOT NULL,
    overlay_geo_opt_desc text NOT NULL
);


ALTER TABLE postgis.overlay_geo_opt OWNER TO postgres;

--
-- TOC entry 205 (class 1259 OID 17460)
-- Name: pirep; Type: TABLE; Schema: postgis; Owner: postgres
--

CREATE TABLE postgis.pirep (
    rep_type character varying(20) NOT NULL,
    fl_lev character varying(10),
    ac_type character varying(10),
    cloud character varying(50),
    weather character varying(50),
    temperature character varying(10),
    wind_spd_dir character varying(15),
    turbulence character varying(50),
    icing character varying(50),
    remarks character varying(100),
    location character varying(50),
    rep_time character varying(15) NOT NULL,
    stn_call character varying(5) NOT NULL
);


ALTER TABLE postgis.pirep OWNER TO postgres;

--
-- TOC entry 224 (class 1259 OID 21038)
-- Name: sigairmet; Type: TABLE; Schema: postgis; Owner: postgres
--

CREATE TABLE postgis.sigairmet (
    prod_id integer NOT NULL,
    rep_num integer NOT NULL,
    text_data character varying,
    stn_call character varying(5) NOT NULL,
    rep_time character varying
);


ALTER TABLE postgis.sigairmet OWNER TO postgres;

--
-- TOC entry 206 (class 1259 OID 17463)
-- Name: stations; Type: TABLE; Schema: postgis; Owner: postgres
--

CREATE TABLE postgis.stations (
    coords public.geometry NOT NULL,
    stn_call character(5) NOT NULL,
    stn_loc character(70) NOT NULL,
    lat numeric NOT NULL,
    lng numeric NOT NULL,
    state character(3) NOT NULL
);


ALTER TABLE postgis.stations OWNER TO postgres;

--
-- TOC entry 232 (class 1259 OID 29657)
-- Name: sua; Type: TABLE; Schema: postgis; Owner: postgres
--

CREATE TABLE postgis.sua (
    rep_time character(10) NOT NULL,
    rep_year integer NOT NULL,
    rep_num integer NOT NULL,
    sched_id integer,
    airsp_id integer,
    sched_status character(2) NOT NULL,
    airsp_name character(35),
    start_time character(20) NOT NULL,
    end_time character(20) NOT NULL,
    high_alt integer,
    sep_rule character(2),
    shape_ind character(2),
    nfdc_id character(20),
    nfdc_name character(20),
    dafif_id character(20),
    dafif_name character(75),
    airsp_type character(2),
    low_alt integer
);


ALTER TABLE postgis.sua OWNER TO postgres;

--
-- TOC entry 208 (class 1259 OID 17504)
-- Name: taf; Type: TABLE; Schema: postgis; Owner: postgres
--

CREATE TABLE postgis.taf (
    issued character varying NOT NULL,
    current character varying,
    wind character varying,
    visby character varying,
    condx character varying,
    rep_time character varying NOT NULL,
    stn_call character varying(5) NOT NULL
);


ALTER TABLE postgis.taf OWNER TO postgres;

--
-- TOC entry 4427 (class 0 OID 17416)
-- Dependencies: 200
-- Data for Name: airspace_ob_ele; Type: TABLE DATA; Schema: postgis; Owner: postgres
--

COPY postgis.airspace_ob_ele (airspace_ob_ele_id, airspace_ob_ele_desc) FROM stdin;
0	Temporary Flight Restriction
1	Turbulence
2	Low Level Wind Shear
3	Surface Winds
4	Icing
5	Freezing Level
6	IFR Conditions
7	Mountain Obscuration
8	Future Use (8)
9	Future Use(9)
10	Future Use(10)
11	Future Use(11)
12	Future Use(12)
13	Future Use(13)
14	Future Use(14)
15	Future Use(15)
16	Future Use(16)
17	Future Use(17)
18	Future Use(18)
19	Future Use(19)
20	Future Use(20)
21	Future Use(21)
22	Future Use(22)
23	Future Use(23)
24	Future Use(24)
25	Future Use(25)
26	Future Use(26)
27	Future Use(27)
28	Future Use(28)
29	Future Use(29)
30	Future Use(30)
31	Future Use(31)
\.


--
-- TOC entry 4434 (class 0 OID 17495)
-- Dependencies: 207
-- Data for Name: circles; Type: TABLE DATA; Schema: postgis; Owner: postgres
--

COPY postgis.circles (alt_top, alt_bot, alpha, prod_id, rec_count, rep_num, rep_year, start_date, stop_date, geo_opt, r_lng, r_lat, bot, top) FROM stdin;
\.


--
-- TOC entry 4428 (class 0 OID 17422)
-- Dependencies: 201
-- Data for Name: fisb_products; Type: TABLE DATA; Schema: postgis; Owner: postgres
--

COPY postgis.fisb_products (fisb_product_id, fisb_product_desc) FROM stdin;
8	NOTAM
11	AIRMET
12	SIGMET
13	SUA
14	G-AIRMET
15	CWA
63	Regional NEXRAD
64	CONUS NEXRAD
70	Icing Forecast - Low
71	Icing Forecast - High
84	Cloud Tops
90	Turbulence Forecast - Low
91	Turbulence Forecast - High
103	Lightning
413	Generic Textual Data Product
\.


--
-- TOC entry 4437 (class 0 OID 21049)
-- Dependencies: 225
-- Data for Name: graphics; Type: TABLE DATA; Schema: postgis; Owner: postgres
--

COPY postgis.graphics (coords, alt, ob_ele, rep_num, prod_id, start_date, stop_date, geo_overlay_opt, stn_call) FROM stdin;
\.


--
-- TOC entry 4429 (class 0 OID 17434)
-- Dependencies: 202
-- Data for Name: metar; Type: TABLE DATA; Schema: postgis; Owner: postgres
--

COPY postgis.metar (stn_call, ob_date, winddir, temp, dewp, visby, windsp, altimeter) FROM stdin;
\.


--
-- TOC entry 4439 (class 0 OID 30864)
-- Dependencies: 233
-- Data for Name: nexrad; Type: TABLE DATA; Schema: postgis; Owner: postgres
--

COPY postgis.nexrad (intensity, coords, altitude, prod_id, cc, block_num, maptime, ice_sld, ice_prob) FROM stdin;
\.


--
-- TOC entry 4431 (class 0 OID 17454)
-- Dependencies: 204
-- Data for Name: overlay_geo_opt; Type: TABLE DATA; Schema: postgis; Owner: postgres
--

COPY postgis.overlay_geo_opt (overlay_geo_opt_id, overlay_geo_opt_desc) FROM stdin;
0	No Geometry
1	High Resolution 3D Polygon
2	Extended Range 3D Polygon (MSL)
3	Extended Range 3D Polygon (AGL)
4	Low Resolution 2D Ellipse
5	High Resolution 3D Ellipse
6	Extended Range Circular Prism (MSL)
7	Extended Range Circular Prism (AGL)
8	Extended Range 3D Point (AGL)
9	Extended Range 3D Point (MSL)
10	Extended Range 3D Polyline (MSL)
11	Extended Range 3D Polyline (AGL)
12	Future Use
13	Future Use
14	Future Use
\.


--
-- TOC entry 4432 (class 0 OID 17460)
-- Dependencies: 205
-- Data for Name: pirep; Type: TABLE DATA; Schema: postgis; Owner: postgres
--

COPY postgis.pirep (rep_type, fl_lev, ac_type, cloud, weather, temperature, wind_spd_dir, turbulence, icing, remarks, location, rep_time, stn_call) FROM stdin;
\.


--
-- TOC entry 4436 (class 0 OID 21038)
-- Dependencies: 224
-- Data for Name: sigairmet; Type: TABLE DATA; Schema: postgis; Owner: postgres
--

COPY postgis.sigairmet (prod_id, rep_num, text_data, stn_call, rep_time) FROM stdin;
\.


--
-- TOC entry 4433 (class 0 OID 17463)
-- Dependencies: 206
-- Data for Name: stations; Type: TABLE DATA; Schema: postgis; Owner: postgres
--

COPY postgis.stations (coords, stn_call, stn_loc, lat, lng, state) FROM stdin;
0101000020E6100000A01A2FDD24F263C09A99999999594F40	PAHX 	Shageluk Airport-Shageluka, AK                                        	62.7	-159.567	AK 
0101000020E6100000355EBA490CD260C004560E2DB22D4D40	PAJN 	Juneau International Airport-Juneau, AK                               	58.357	-134.564	AK 
0101000020E6100000DF4F8D976E2E62C03BDF4F8D973E4E40	PACV 	Merle K. (Mudhole) Smith Airport-Cordova, AK                          	60.489	-145.451	AK 
0101000020E610000077BE9F1A2FA563C046B6F3FDD4B85040	PAGH 	Shungnak Airport-Shungnak, AK                                         	66.888	-157.162	AK 
0101000020E6100000F4FDD478E9BE55C0CDCCCCCCCC2C4040	KSEM 	Selma Airport-Selma, AL                                               	32.35	-86.983	AL 
0101000020E6100000DD240681959355C0BC74931804964040	KSCD 	Sylacauga, AL                                                         	33.172	-86.306	AL 
0101000020E6100000EE7C3F355EB255C0DF4F8D976E524140	KHSV 	Huntsville International Airport, AL                                  	34.644	-86.787	AL 
0101000020E6100000EBC5504EB47152C08BFD65F7E4514440	K06A 	Moton Field Municipal Airport-Tuskegee, AL                            	40.6398	-73.77663	AL 
0101000020E6100000E3C798BB96A056C00EBE30992A084240	KPGR 	Kirk Field Airport Paragould, AR                                      	36.0638	-90.5092	AR 
0101000020E6100000FCA9F1D24DFA5BC039B4C876BECF4040	KSDL 	Scottsdale Airport-Scottsdale, AZ                                     	33.623	-111.911	AZ 
0101000020E61000004C378941604D5BC0D122DBF97E7A4040	KCFT 	Greenlee County Airport Clifton/Morenci, AZ                           	32.957	-109.209	AZ 
0101000020E6100000C520B07268915CC0F90FE9B7AF134140	KP20 	Avi Suquilla-Parker, AZ                                               	34.1538	-114.272	AZ 
0101000020E61000004A0C022B87665DC06DE7FBA9F1F24040	KAJO 	Corona Municipal Airport-Corona, CA                                   	33.898	-117.602	CA 
0101000020E610000062105839B4685DC0CDCCCCCCCCFC4040	KCNO 	Chino Airport-Chino, CA                                               	33.975	-117.636	CA 
0101000020E6100000F4FDD478E9965DC07D3F355EBA194140	KBUR 	Bob Hope Airport Burbank, CA                                          	34.201	-118.358	CA 
0101000020E6100000736891ED7C875EC0B29DEFA7C6BB4240	KPAO 	Palo Alto Airport-Santa Clara, CA                                     	37.467	-122.117	CA 
0101000020E61000000000000000905EC01904560E2DC24240	KSQL 	San Carlos Airport-San Carlos, CA                                     	37.517	-122.25	CA 
0101000020E61000008FC2F5285C975EC08FC2F5285CCF4240	KSFO 	San Francisco International-Burlingame, CA                            	37.62	-122.365	CA 
0101000020E61000005839B4C8764E5EC06F1283C0CAF14240	KSCK 	Stockton Metropolitan Airport-Stockton, CA                            	37.889	-121.226	CA 
0101000020E6100000643BDF4F8D775DC04A0C022B87D64040	KSNA 	John Wayne Airport Santa Ana, CA                                      	33.676	-117.868	CA 
0101000020E6100000A8C64B3789F95AC054E3A59BC4404340	KMTJ 	Montrose Regional-Montrose, CO                                        	38.506	-107.899	CO 
0101000020E6100000289B7285776752C0545227A089884440	K5B3 	Putnam, CT                                                            	41.0667	-73.61667	CT 
0101000020E61000005BCEA5B8AA0A52C00000000000E04440	K5B0 	Mansfield, CT                                                         	41.75	-72.16667	CT 
0101000020E6100000E3A59BC4204852C0A245B6F3FD944440	KBDR 	Igor I. Sikorsky Memorial Airport-Bridgeport, CT                      	41.164	-73.127	CT 
0101000020E610000054E3A59BC43852C06F1283C0CAA14440	KHVN 	Tweed-New Haven Airport-New Haven, CT                                 	41.264	-72.887	CT 
0101000020E61000004260E5D0220352C0448B6CE7FBA94440	KGON 	Groton-New London Airport-Groton / New London, CT                     	41.328	-72.049	CT 
0101000020E6100000F4FDD478E95E52C0736891ED7CAF4440	KDXR 	Danbury Municipal Airport-Danbury, CT                                 	41.371	-73.483	CT 
0101000020E6100000AAF1D24D622052C0FED478E926B14440	KSNC 	Chester Airport-Chester, CT                                           	41.384	-72.506	CT 
0101000020E61000008D976E12834852C0E7FBA9F1D2BD4440	KOXC 	Waterbury-Oxford Airport-Oxford, CT                                   	41.483	-73.133	CT 
0101000020E6100000A245B6F3FD3452C0E17A14AE47C14440	KMMK 	Meriden Markham Municipal Airport-Meriden, CT                         	41.51	-72.828	CT 
0101000020E61000008B6CE7FBA92952C091ED7C3F35DE4440	KHFD 	Hartford-Brainard Airport-Hartford, CT                                	41.736	-72.651	CT 
0101000020E6100000B29DEFA7C60B52C0E5D022DBF9DE4440	KIJD 	Windham Airport-Willimantic, CT                                       	41.742	-72.184	CT 
0101000020E6100000CFF753E3A52B52C0F2D24D6210F84440	KBDL 	Bradley International Airport-Windsor Locks, CT                       	41.938	-72.682	CT 
0101000020E61000000B98C0ADBB3B53C0CDCCCCCCCC6C4340	KWBC 	News Headquarters, DC                                                 	38.85	-76.93333	DC 
0101000020E610000033333333336353C0CDCCCCCCCC8C4340	KZDC 	Washington-ARTCC, DC                                                  	39.1	-77.55	DC 
0101000020E6100000C1CAA145B6DB52C0B4C876BE9F6A4340	KDOX 	Dover-Ellendale, DE                                                   	38.833	-75.433	DE 
0101000020E61000006666666666E652C088855AD3BC9B4340	K33N 	Dover-Cheswold Airpark, DE                                            	39.2167	-75.6	DE 
0101000020E61000003F355EBA491454C07D3F355EBAC93940	KMIA 	Miami International Airport, FL                                       	25.788	-80.317	FL 
0101000020E6100000B29DEFA7C6A354C02DB29DEFA7663B40	KSRQ 	Sarasota-Bradenton-Sarasota,Fl                                        	27.401	-82.559	FL 
0101000020E61000008716D9CEF75B54C00AD7A3703D4A3C40	KISM 	Kissimmee Gateway-Orlando, FL                                         	28.29	-81.437	FL 
0101000020E6100000560E2DB29D4F54C048E17A14AEC73C40	KSFB 	Orlando Sanford International Airport-Orlando, FL                     	28.78	-81.244	FL 
0101000020E610000023DBF97E6A6C54C01F85EB51B87E3E40	KJAX 	Jacksonville International Airport, Jacksonville, FL                  	30.495	-81.694	FL 
0101000020E6100000C2340C1F117154C06666666666263A40	KAPF 	Naples Airplort, FL                                                   	26.15	-81.76667	FL 
0101000020E61000003CBD5296217A54C06DE7FBA9F1B23E40	KZJX 	Hilliard Jacksonville ARTCC, FL                                       	30.699	-81.9083	FL 
0101000020E6100000B0726891ED4C54C054E3A59BC4104040	KSAV 	Savannah/Hilton Head International Airport, GA                        	32.131	-81.202	GA 
0101000020E61000003F355EBA493C55C0355EBA490C424040	KCSG 	Columbus Airport, COlumbus, GA                                        	32.516	-84.942	GA 
0101000020E61000001B2FDD2406A154C0A4703D0AD7C34040	KHQU 	Thomson-Mcduffie County Airpot, Thomson, GA                           	33.53	-82.516	GA 
0101000020E610000017D9CEF7531355C00000000000F04040	KPDK 	DeKalb-Peachtree Airport-Chamblee, GA                                 	33.875	-84.302	GA 
0101000020E6100000289B728577B754C024287E8CB97B3F40	KDQH 	Douglas Municipal Gene Chambers Airport-Douglas, GA                   	31.4833	-82.86667	GA 
0101000020E6100000CDCCCCCCCC345CC0A01A2FDD24664540	KU01 	American Falls Airport-American Falls, ID                             	42.798	-112.825	ID 
0101000020E6100000B29DEFA7C66B56C05C8FC2F528EC4340	KSPI 	Abraham Lincoln Capital Airport, Springfield, IL                      	39.845	-89.684	IL 
0101000020E6100000D8648D7A88F855C00000000000004540	KCHI 	Chicago-WSFO, IL                                                      	42	-87.88333	IL 
0101000020E6100000B29DEFA7C66B56C02FDD240681554440	KPIA 	General Downing-Peoria International Airport, Peoria, IL              	40.668	-89.684	IL 
0101000020E6100000A69BC420B00A56C00000000000C04440	KJOT 	Joliet Regional Airport, Joliet,Il                                    	41.5	-88.167	IL 
0101000020E6100000CFF753E3A5FB55C07B14AE47E1FA4440	KORD 	O"Hare International-Chicago, IL                                      	41.96	-87.932	IL 
0101000020E61000003D0AD7A3707556C0F6285C8FC2454340	KBLV 	Scott AFB/Midamerica Airport Belleville, IL                           	38.545	-89.835	IL 
0101000020E6100000D34D621058E155C01283C0CAA1054340	KEVV 	Evansville Regional Airport, IN                                       	38.044	-87.521	IN 
0101000020E6100000B29DEFA7C6D355C060E5D022DBB94340	KHUF 	Terre Haute Regional Airport-Terre Haute, IN                          	39.452	-87.309	IN 
0101000020E6100000355EBA490C9255C0CDCCCCCCCCDC4340	KIND 	Indianapolis International Airport, IN                                	39.725	-86.282	IN 
0101000020E6100000C2340C1F11B155C088855AD3BC5B4440	KMCX 	Montcello/White, IN                                                   	40.7167	-86.76667	IN 
0101000020E610000077BE9F1A2F4D55C03F355EBA497C4440	KFWA 	Fort Wayne, IN                                                        	40.971	-85.206	IN 
0101000020E61000003F355EBA49A45AC04C37894160954140	KZAB 	Albuquerque Center Weather Service Unit, NM                           	35.167	-106.567	NM 
0101000020E61000006FF085C954855DC0454772F90F514140	KZLA 	Los Angeles Center Weather Service Unit , CA                          	34.6333	-118.0833	CA 
0101000020E61000000B98C0ADBB4B57C0454772F90F514640	KZMP 	Minneapolis Center Weather Service Unit, MN                           	44.6333	-93.18333	MN 
0101000020E61000004260E5D0229B55C0EC51B81E857B4240	KBWG 	Bowling Green-Warren County Regional Airport-Bowling Green, KY        	36.965	-86.424	KY 
0101000020E61000009EEFA7C64B6F55C0BA490C022B174340	KSDF 	Louisville International, KY                                          	38.181	-85.739	KY 
0101000020E6100000D122DBF97E6A55C077BE9F1A2F1D4340	KLOU 	Bowman Field Airport, Louisville, KY                                  	38.228	-85.664	KY 
0101000020E61000005EBA490C022B55C01283C0CAA1854340	KCVG 	Covington, KY                                                         	39.044	-84.672	KY 
0101000020E6100000B4C876BE9F0257C0355EBA490C424040	KMLU 	Monroe Regional Airport-Monroe, LA                                    	32.516	-92.041	LA 
0101000020E6100000D8648D7A88C851C09A99999999F94440	KBOX 	Boston-Taunton, MA                                                    	41.95	-71.13333	MA 
0101000020E6100000D8648D7A88C851C09A99999999F94440	KTAR 	Boston-RFC, MA                                                        	41.95	-71.13333	MA 
0101000020E61000005BCEA5B8AAAA51C0CDCCCCCCCC0C4540	K3B2 	Marshfield, MA                                                        	42.1	-70.66667	MA 
0101000020E610000066666666665652C09A99999999194540	KGBR 	Walter J. Koladza Airport-Great Barrington, MA                        	42.2	-73.35	MA 
0101000020E6100000289B7285773752C0EEEBC03923224540	K7B2 	Knightville, MA                                                       	42.2667	-72.86667	MA 
0101000020E61000005BCEA5B8AA3A52C0454772F90F314540	KWOR 	Worthington, MA                                                       	42.3833	-72.91667	MA 
0101000020E6100000F5673F52440452C0211FF46C56354540	K8B5 	Barre Falls, MA                                                       	42.4167	-72.06667	MA 
0101000020E61000008E01D9EBDDFD51C0545227A089484540	KGDM 	Gardner Municipal Airport-Gardner, MA                                 	42.5667	-71.96667	MA 
0101000020E6100000289B728577F751C03333333333534540	K9B4 	Ashburnham, MA                                                        	42.65	-71.86667	MA 
0101000020E61000000B98C0ADBB4B52C09A99999999594540	K2B6 	North Adams, MA                                                       	42.7	-73.18333	MA 
0101000020E61000005BCEA5B8AABA51C0545227A089684540	K2B2 	Newburyport, MA                                                       	42.8167	-70.91667	MA 
0101000020E610000096438B6CE78351C0AAF1D24D62A04440	KACK 	Nantucket Memorial Airport-Nantucket, MA                              	41.253	-70.061	MA 
0101000020E61000008FC2F5285CA751C0FCA9F1D24DB24440	KMVY 	Martha"S Vineyard Airport-Vineyard Haven, MA                          	41.393	-70.615	MA 
0101000020E61000000C022B8716A151C03333333333D34440	KFMH 	Otis Air National Guard Base-Falmouth, MA                             	41.65	-70.517	MA 
0101000020E6100000D9CEF753E37D51C04C37894160D54440	KCHH 	Chatham, MA                                                           	41.667	-69.967	MA 
0101000020E610000052B81E85EB9151C01283C0CAA1D54440	KHYA 	Barnstable Municipal Airport-Hyannis, MA                              	41.669	-70.28	MA 
0101000020E61000005A643BDF4FBD51C04A0C022B87D64440	KEWB 	New Bedford Regional Airport-New Bedford, MA                          	41.676	-70.958	MA 
0101000020E6100000643BDF4F8D7F51C0F2D24D6210D84440	KCQX 	Chatham Municipal Airport-Chatham, MA                                 	41.688	-69.993	MA 
0101000020E6100000D34D621058C151C0E3A59BC420F04440	KTAN 	Taunton Municipal Airport-Taunton, MA                                 	41.876	-71.021	MA 
0101000020E61000002DB29DEFA7AE51C014AE47E17AF44440	KPYM 	Plymouth Municipal Airport-Plymouth, MA                               	41.91	-70.729	MA 
0101000020E6100000A01A2FDD248E51C0F0A7C64B37094540	KPVC 	Provincetown Municipal Airport-Provincetown, MA                       	42.072	-70.221	MA 
0101000020E61000005EBA490C02AB51C0068195438B0C4540	KGHG 	Marshfield Municipal Airport -Marshfield, MA                          	42.098	-70.672	MA 
0101000020E6100000C1CAA145B6BB51C03333333333134540	KNZW 	South Weymouth, MA                                                    	42.15	-70.933	MA 
0101000020E6100000E7FBA9F1D22D52C04E62105839144540	KBAF 	Westfield-Barnes Regional Airport-Springfield, MA                     	42.158	-72.716	MA 
0101000020E61000004260E5D022CB51C09CC420B072184540	KOWD 	Norwood Memorial Airport-Norwood, MA                                  	42.191	-71.174	MA 
0101000020E6100000273108AC1C2252C09A99999999194540	KCEF 	Westover Metropolitan Airport-Springfield, MA                         	42.2	-72.533	MA 
0101000020E61000009EEFA7C64BC751C04260E5D0221B4540	KMQE 	Milton/Blue Hills Observatory, MA                                     	42.212	-71.114	MA 
0101000020E61000001D5A643BDFF751C0A69BC420B0224540	KORH 	Worcester Regional Airport-Worcester, MA                              	42.271	-71.873	MA 
0101000020E6100000713D0AD7A3C051C091ED7C3F352E4540	KBOS 	Logan International Airport-Boston, MA                                	42.361	-71.01	MA 
0101000020E6100000D122DBF97E5252C02DB29DEFA7364540	KPSF 	Pittsfield Municipal Airport-Pittsfield, MA                           	42.427	-73.289	MA 
0101000020E6100000D122DBF97ED251C05C8FC2F5283C4540	KBED 	Laurence G. Hanscom Field-Bedford, MA                                 	42.47	-71.289	MA 
0101000020E6100000AAF1D24D62F051C02DB29DEFA7464540	KFIT 	Fitchburg Municipal Airport-Fitchburg, MA                             	42.552	-71.756	MA 
0101000020E61000006666666666E651C07F6ABC7493484540	KAYE 	Ayer, MA                                                              	42.567	-71.6	MA 
0101000020E6100000B4C876BE9F1252C0295C8FC2F5484540	KORE 	Orange Municipal Airport-Orange, MA                                   	42.57	-72.291	MA 
0101000020E6100000986E1283C0BA51C0986E1283C04A4540	KBVY 	Beverly Regional Airport-Beverly, MA                                  	42.584	-70.918	MA 
0101000020E61000007B14AE47E14A52C0F0A7C64B37594540	KAQW 	Harriman-And-West Airport-North Adams, MA                             	42.697	-73.17	MA 
0101000020E61000000E2DB29DEFC751C0B29DEFA7C65B4540	KLWM 	Lawrence Municipal Airport-Lawrence, MA                               	42.717	-71.124	MA 
0101000020E6100000FA7E6ABC741B53C01904560E2D124340	KNUI 	Naval Outlying Field Webster-Saint Inigoes, MD                        	38.142	-76.429	MD 
0101000020E6100000A69BC420B01A53C06666666666264340	KNHK 	Naval Air Station Patuxent River, MD                                  	38.3	-76.417	MD 
0101000020E61000000E2DB29DEFC752C08195438B6C274340	KOXB 	Ocean City Municipal Airport-Ocean City, MD                           	38.308	-75.124	MD 
0101000020E610000046B6F3FDD4E052C0CFF753E3A52B4340	KSBY 	Salisbury-Ocean City Regional Airport-Salisbury, MD                   	38.341	-75.513	MD 
0101000020E6100000273108AC1C0253C04E62105839444340	KCGE 	Cambridge-Dorchester Regional Airport-Cambridge, MD                   	38.533	-76.033	MD 
0101000020E610000023DBF97E6A0453C0F4FDD478E9664340	KESN 	Easton Airport (Newnan Field)-Easton, MD                              	38.804	-76.069	MD 
0101000020E6100000736891ED7C3753C07F6ABC7493684340	KADW 	Andrews Air Force Base-Camp Springs, MD                               	38.817	-76.867	MD 
0101000020E61000005A643BDF4F1553C0B29DEFA7C67B4340	KW29 	Stevensville, MD                                                      	38.967	-76.333	MD 
0101000020E61000008195438B6CF752C05C8FC2F5287C4340	KRJD 	Ridgely Airpark-Ridgely, MD                                           	38.97	-75.866	MD 
0101000020E61000005EBA490C023B53C021B07268917D4340	KCGS 	College Park Airport-College Park, MD                                 	38.981	-76.922	MD 
0101000020E61000001F85EB51B81E53C01F85EB51B87E4340	KNAK 	Annapolis/Naval Academy, MD                                           	38.99	-76.48	MD 
0101000020E61000000C022B87163153C0B4C876BE9F8A4340	KFME 	Tipton Airport-Fort Meade / Odenton, MD                               	39.083	-76.767	MD 
0101000020E6100000A69BC420B04A53C04C37894160954340	KGAI 	Montgomery County Airpark-Gaithersburg, MD                            	39.167	-77.167	MD 
0101000020E6100000B29DEFA7C62B53C0A01A2FDD24964340	KBWI 	Baltimore-Washington International-Baltimore, MD                      	39.173	-76.684	MD 
0101000020E6100000C976BE9F1A2753C08716D9CEF7A34340	KDMH 	Baltimore-Dwntwn, MD                                                  	39.281	-76.611	MD 
0101000020E6100000A69BC420B01A53C0B4C876BE9FAA4340	KMTN 	Martin State Airport-Baltimore, MD                                    	39.333	-76.417	MD 
0101000020E61000008D976E12835853C04C37894160B54340	KFDK 	Frederick Municipal Airport-Frederick, MD                             	39.417	-77.383	MD 
0101000020E61000007B14AE47E10A53C023DBF97E6ABC4340	KAPG 	Phillips Army Airfield-Aberdeen Proving Ground, MD                    	39.472	-76.17	MD 
0101000020E61000000C022B8716B153C0CDCCCCCCCCCC4340	KCBE 	Greater Cumberland Regional Airport-Cumberland, MD                    	39.6	-78.767	MD 
0101000020E61000008D976E12834053C0E7FBA9F1D2CD4340	KDMW 	Carroll County Regional Airport-Westminster, MD                       	39.608	-77.008	MD 
0101000020E6100000CBA145B6F35D53C0C3F5285C8FD24340	KRSP 	NSF Thurmont, MD                                                      	39.645	-77.468	MD 
0101000020E61000001F85EB51B86E53C0EE7C3F355EDA4340	KHGR 	Hagerstown Regional Airport-Hagerstown, MD                            	39.706	-77.73	MD 
0101000020E610000071AC8BDB682453C0FFB27BF2B0784340	KANP 	Lee Airport-Annapolis, MD                                             	38.9429	-76.5689	MD 
0101000020E61000009FCDAACFD51253C027C286A757B24340	KEDG 	Weide Army Heliport-Edgewood Arsenal, Aberdeen Proving Ground,MD      	39.3933	-76.2943	MD 
0101000020E6100000F0A7C64B373D53C09CA223B9FC5F4340	KVKX 	Potomac Airfield-Friendly, MD                                         	38.7499	-76.9565	MD 
0101000020E610000033333333332353C0EACF7EA488284340	K2W6 	St Marys (Duke), MD                                                   	38.31667	-76.55	MD 
0101000020E6100000289B728577C752C0EACF7EA488284340	KN80 	Ocean City, MD                                                        	38.31667	-75.11667	MD 
0101000020E61000000B98C0ADBB3B53C00000000000604340	KW32 	Clinton, MD                                                           	38.75	-76.93333	MD 
0101000020E61000000B98C0ADBB3B53C0CDCCCCCCCC6C4340	KWNB 	Camp Springs-DBC, MD                                                  	38.85	-76.93333	MD 
0101000020E61000000B98C0ADBB3B53C0CDCCCCCCCC6C4340	KWNC 	Camp Springs-CPC, MD                                                  	38.85	-76.93333	MD 
0101000020E61000000B98C0ADBB3B53C0CDCCCCCCCC6C4340	KWNH 	Camp Springs-HPC, MD                                                  	38.85	-76.93333	MD 
0101000020E61000000B98C0ADBB3B53C0CDCCCCCCCC6C4340	KWNM 	Camp Springs-MPC, MD                                                  	38.85	-76.93333	MD 
0101000020E61000000B98C0ADBB3B53C0CDCCCCCCCC6C4340	KWNO 	Camp Spring-NCEP, MD                                                  	38.85	-76.93333	MD 
0101000020E61000003ECBF3E0EE3E53C00000000000C04340	KEMI 	Westminster, MD                                                       	39.5	-76.98333	MD 
0101000020E6100000A5315A4755D553C04963B48EAACA4340	K2G4 	Oakland/Garrett County, MD                                            	39.58333	-79.33333	MD 
0101000020E61000008E01D9EBDD5D53C03333333333D34340	KDVD 	Camp David, MD                                                        	39.65	-77.46667	MD 
0101000020E61000005BCEA5B8AA5A53C01D03B2D7BBDB4340	K43M 	Fort Ritchie-Sit, MD                                                  	39.71667	-77.41667	MD 
0101000020E61000005BCEA5B8AA5A53C01D03B2D7BBDB4340	KJWX 	Fort Ritchie, MD                                                      	39.71667	-77.41667	MD 
0101000020E6100000FFB27BF2B04051C0143FC6DCB5344640	KBST 	Belfast Municipal Airport-Belfast, ME                                 	44.4118	-69.0108	ME 
0101000020E61000006F1283C0CA7B51C097FF907EFBF24540	KBXM 	Brunswick Executive Airport-Brunswick, ME                             	43.8983	-69.93425	ME 
0101000020E610000075029A081B2251C0E08442041CAE4640	KLRG 	Lincoln Regional Airport-Lincoln, ME                                  	45.36023	-68.5329	ME 
0101000020E61000001F4B1FBAA0DE50C07DD0B359F5594640	KMVM 	Machias Valley Airport-Machias, ME                                    	44.7028	-67.47856	ME 
0101000020E61000004CE0D6DD3C7751C000C63368E85B4640	KOWK 	Central Maine Airport-Norridgewock, ME                                	44.71803	-69.86309	ME 
0101000020E6100000A301BC0512E450C06F8104C58F994640	KPNN 	Princeton Municipal Airport-Princeton, ME                             	45.1997	-67.5636	ME 
0101000020E610000000000000000051C00000000000804540	K9B6 	Sewel Ridge, ME                                                       	43	-68	ME 
0101000020E6100000289B728577A751C01630815B77B74540	KENE 	Kennebunk Port, ME                                                    	43.43333	-70.61667	ME 
0101000020E610000000000000009051C0B0C91AF510F14540	KGYX 	Gray-Portland, ME                                                     	43.88333	-70.25	ME 
0101000020E61000008E01D9EBDD6D51C01D03B2D7BBFB4540	K9B9 	Wiscasset, ME                                                         	43.96667	-69.71667	ME 
0101000020E6100000CDCCCCCCCCBC51C07D96E7C1DDFD4540	KB20 	Freyburg, ME                                                          	43.98333	-70.95	ME 
0101000020E610000072FE261422A251C0E3FC4D2844444640	KRUM 	Rumford, ME                                                           	44.53333	-70.53333	ME 
0101000020E61000000000000000C050C0B79C4B7155754640	K47B 	Eastport, ME                                                          	44.91667	-67	ME 
0101000020E6100000289B7285776751C01630815B77774640	K2B7 	Harmony, ME                                                           	44.93333	-69.61667	ME 
0101000020E61000008E01D9EBDD8D51C04963B48EAA8A4640	KB21 	Carrabassett, ME                                                      	45.08333	-70.21667	ME 
0101000020E610000033333333336351C01D03B2D7BBBB4640	K3B1 	Greenville, ME                                                        	45.46667	-69.55	ME 
0101000020E61000003333333333F350C0E3FC4D2844044740	KCBW 	Houlton,-88D, ME                                                      	46.03333	-67.8	ME 
0101000020E61000005A643BDF4FAD51C0DF4F8D976EB24540	KSFM 	Sanford Seacoast Regional Airport-Sanford, ME                         	43.394	-70.708	ME 
0101000020E6100000FA7E6ABC749351C01904560E2DD24540	KPWM 	Portland International Jetport-Portland, ME                           	43.642	-70.304	ME 
0101000020E6100000C1CAA145B67B51C03333333333F34540	KNHZ 	Brunswick NAS, ME                                                     	43.9	-69.933	ME 
0101000020E610000021B07268916D51C008AC1C5A64FB4540	KIWI 	Wiscasset Airport-Wiscasset, ME                                       	43.964	-69.712	ME 
0101000020E6100000E9263108ACBC51C0022B8716D9FE4540	KIZG 	Eastern Slopes Regional Airport-Fryeburg, ME                          	43.991	-70.948	ME 
0101000020E6100000273108AC1C9251C06666666666064640	KLEW 	Auburn/Lewiston Municipal Airport-Auburn, ME                          	44.05	-70.283	ME 
0101000020E610000066666666664651C07F6ABC7493084640	KRKD 	Knox County Regional Airport-Rockland, ME                             	44.067	-69.1	ME 
0101000020E61000005EBA490C027351C09CC420B072284640	KAUG 	Augusta State Airport-Augusta, ME                                     	44.316	-69.797	ME 
0101000020E6100000736891ED7C1751C09A99999999394640	KBHB 	Hancock County-Bar Harbor Airport-Bar Harbor, ME                      	44.45	-68.367	ME 
0101000020E6100000A69BC420B06A51C04E62105839444640	KWVL 	Waterville Robert Lafleur Airport-Waterville, ME                      	44.533	-69.667	ME 
0101000020E610000023DBF97E6A3451C0A01A2FDD24664640	KBGR 	Bangor International Airport-Bangor, ME                               	44.798	-68.819	ME 
0101000020E61000000000000000C050C04C37894160754640	KEPM 	Eastport Municipal Airport-Eastport, ME                               	44.917	-67	ME 
0101000020E6100000A69BC420B02A51C09A99999999794640	KOLD 	Old Town Municipal Airport-Old Town, ME                               	44.95	-68.667	ME 
0101000020E6100000C3F5285C8FAA51C0E5D022DBF97E4640	K8B0 	Rangeley, ME                                                          	44.992	-70.665	ME 
0101000020E6100000AE47E17A146651C04260E5D022BB4640	KGNR 	Greenville Municipal, ME                                              	45.462	-69.595	ME 
0101000020E61000003108AC1C5A2C51C06DE7FBA9F1D24640	KMLT 	Millinocket Municipal Airport-Millinocket, ME                         	45.648	-68.693	ME 
0101000020E6100000986E1283C0F250C0AC1C5A643B0F4740	KHUL 	Houlton International Airport-Houlton, ME                             	46.119	-67.793	ME 
0101000020E6100000273108AC1C6251C0E5D022DBF94E4740	K40B 	Clayton Lake, ME                                                      	46.617	-69.533	ME 
0101000020E610000033333333330351C08195438B6C574740	KPQI 	Northern Maine Regional Airport-Presque Isle, ME                      	46.683	-68.05	ME 
0101000020E61000000C022B87160151C0736891ED7C6F4740	KCAR 	Caribou Municipal Airport-Caribou, ME                                 	46.871	-68.017	ME 
0101000020E61000008D976E1283F850C09A99999999794740	KLIZ 	Loring AFB, ME                                                        	46.95	-67.883	ME 
0101000020E610000079E92631081451C0F853E3A59BA44740	KFVE 	Northern Aroostook Regional Airport-Frenchville, ME                   	47.286	-68.313	ME 
0101000020E61000008E01D9EBDDAD54C00000000000A04540	KECK 	Peck, MI                                                              	43.25	-82.71667	MI 
0101000020E6100000289B7285771755C01D03B2D7BB3B4740	KSSM 	Sault Ste Marie, MI                                                   	46.46667	-84.36667	MI 
0101000020E6100000BA490C022B8F55C0D9CEF753E3954540	KMKG 	Muskegon County Airport, MI                                           	43.171	-86.237	MI 
0101000020E61000005A643BDF4F6555C0022B8716D95E4640	KTVC 	Cherry Capital Airport, Traverse City, MI                             	44.741	-85.583	MI 
0101000020E61000004260E5D022E355C08716D9CEF7434740	KMQT 	Marquette, MI                                                         	46.531	-87.549	MI 
0101000020E61000009EEFA7C64B1F56C01283C0CAA1954740	KCMX 	Houghton County Memorial Airport-Hancock, MI                          	47.169	-88.489	MI 
0101000020E61000005DFE43FAED9757C03EEDF0D764314740	K9Y2 	East Gull Lake, MN                                                    	46.38589	-94.3739	MN 
0101000020E61000006ADE718A8E6857C0FB3A70CE88E24640	K18Y 	Milaca Municipal Airport, MN                                          	45.7698	-93.6337	MN 
0101000020E61000000E2DB29DEF9756C0AAF1D24D62604340	KSTL 	St Louis/Lambert International Airport, MO                            	38.753	-90.374	MO 
0101000020E6100000CBA145B6F30D57C07F6ABC7493684340	KCOU 	Columbia Regional Airport-Columbia, MO                                	38.817	-92.218	MO 
0101000020E610000091ED7C3F35A657C0736891ED7C8F4340	KMKC 	Charles B. Wheeler Downtown Airport-Kansas City, MO                   	39.121	-94.597	MO 
0101000020E6100000508D976E12BB57C0508D976E12E34340	KSTJ 	Rosecrans Memorial-St Joseph, MO                                      	39.774	-94.923	MO 
0101000020E6100000ACCABE2B82A756C0BEC1172653E14240	KH57 	Bismark Memorial Airport-Bismark, MO                                  	37.76035	-90.61732	MO 
0101000020E61000005BCEA5B8AAAA57C0E3FC4D2844A44340	KKCI 	Kansas City International Airport, MO                                 	39.28333	-94.66667	MO 
0101000020E61000009CC420B072D85BC0068195438BBC4740	KGTF 	Great Falls, MT                                                       	47.473	-111.382	MT 
0101000020E6100000CDCCCCCCCC8453C0F4FDD478E9F64040	KSUT 	Cape Fear Regional Jetport-Oak Island, NC                             	33.929	-78.075	NC 
0101000020E61000009A999999997953C0FCA9F1D24D224140	KILM 	Wilmington International Airport-Wilmington, NC                       	34.268	-77.9	NC 
0101000020E6100000F6285C8FC2AD53C06DE7FBA9F1224140	KCPC 	Columbus County Municipal Airport-Whiteville, NC                      	34.273	-78.715	NC 
0101000020E6100000A245B6F3FDA453C0931804560E4D4140	KEYF 	Curtis L. Brown Jr. Field-Elizabethtown, NC                           	34.602	-78.578	NC 
0101000020E6100000B29DEFA7C6C353C0E7FBA9F1D24D4140	KLBT 	Lumberton Municipal Airport-Lumberton, NC                             	34.608	-79.059	NC 
0101000020E6100000273108AC1C4253C08195438B6C574140	KNJM 	Marine Corps Auxiliary Landing Field Bogue-Swansboro, NC              	34.683	-77.033	NC 
0101000020E61000005C8FC2F5285C53C0B4C876BE9F5A4140	KNCA 	Marine Corps Air Station New River-Jacksonville, NC                   	34.708	-77.44	NC 
0101000020E6100000355EBA490C2A53C0E7FBA9F1D25D4140	KMRH 	Michael J. Smith Field-Beaufort, NC                                   	34.733	-76.657	NC 
0101000020E6100000736891ED7C3753C01904560E2D624140	KMHX 	Morehead-Newport, NC                                                  	34.767	-76.867	NC 
0101000020E61000008195438B6CD753C04C37894160654140	KMEB 	Laurinburg-Maxton Airport-Maxton, NC                                  	34.792	-79.366	NC 
0101000020E6100000736891ED7C6753C0B4C876BE9F6A4140	KOAJ 	Albert J. Ellis Airport-Jacksonville, NC                              	34.833	-77.617	NC 
0101000020E61000007F6ABC7493F053C0355EBA490C724140	KRCZ 	Rockingham, NC                                                        	34.891	-79.759	NC 
0101000020E61000008D976E12833853C03333333333734140	KNKT 	Marine Corps Air Station Cherry Point, NC                             	34.9	-76.883	NC 
0101000020E61000009EEFA7C64B9753C0B0726891ED7C4140	KCTZ 	Clinton-Sampson County Airport-Clinton, NC                            	34.976	-78.364	NC 
0101000020E6100000B81E85EB51B853C0022B8716D97E4140	KFAY 	Fayetteville Regional Airport-Fayetteville, NC                        	34.991	-78.88	NC 
0101000020E6100000022B8716D97E53C00000000000804140	KDPL 	Duplin County Airport-Kenansville, NC                                 	35	-77.982	NC 
0101000020E61000005A643BDF4F0554C01904560E2D824140	KAFP 	Anson County Airport-Wadesboro, NC                                    	35.017	-80.083	NC 
0101000020E610000039B4C876BE2754C01904560E2D824140	KEQY 	Monroe Regional Airport-Monroe, NC                                    	35.017	-80.621	NC 
0101000020E6100000D9CEF753E31D53C01904560E2D824140	KNBT 	Piney Island/Bomb Range, NC                                           	35.017	-76.467	NC 
0101000020E61000000000000000E053C04E62105839844140	KHFF 	Mackall Army Airfield-Camp Mackall, NC                                	35.033	-79.5	NC 
0101000020E6100000508D976E124353C062105839B4884140	KEWN 	Craven County Regional Airport-New Bern, NC                           	35.068	-77.048	NC 
0101000020E6100000C1CAA145B6BB53C01B2FDD2406914140	KFBG 	Simmons Army Airfield-Fort Bragg, NC                                  	35.133	-78.933	NC 
0101000020E61000007F6ABC7493C053C083C0CAA145964140	KPOB 	Pope Field (Military)-Fayetteville, NC                                	35.174	-79.009	NC 
0101000020E61000008FC2F5285CF754C0295C8FC2F5984140	KRHP 	Western Carolina Regional Airport-Andrews, NC                         	35.195	-83.865	NC 
0101000020E6100000448B6CE7FB4954C0F0A7C64B37994140	KAKH 	Gastonia Municipal Airport-Gastonia, NC                               	35.197	-81.156	NC 
0101000020E6100000894160E5D0DA54C0068195438B9C4140	K1A5 	Macon-Franklin, NC                                                    	35.223	-83.419	NC 
0101000020E610000085EB51B81E3D54C0E9263108AC9C4140	KCLT 	Charlotte/Douglas International Airport-Charlotte, NC                 	35.224	-80.955	NC 
0101000020E61000002B8716D9CEE752C0E7FBA9F1D29D4140	KHSE 	Billy Mitchell Airport-Hatteras, NC                                   	35.233	-75.622	NC 
0101000020E61000001B2FDD2406D953C075931804569E4140	KSOP 	Moore County Airport-Southern Pines, NC                               	35.237	-79.391	NC 
0101000020E61000005839B4C8766654C054E3A59BC4A04140	KEHO 	Shelby-Cleveland Regional Airport-Shelby, NC                          	35.256	-81.601	NC 
0101000020E61000008D976E12836853C07F6ABC7493A84140	KISO 	Kinston Regional Jetport (Stallings Field)-Kinston, NC                	35.317	-77.633	NC 
0101000020E6100000F6285C8FC27D53C079E9263108AC4140	KGSB 	Seymour Johnson Air Force Base-Goldsboro, NC                          	35.344	-77.965	NC 
0101000020E6100000E5D022DBF9AE53C08D976E1283B04140	KHRJ 	Harnett Regional Jetport-Erwin, NC                                    	35.379	-78.734	NC 
0101000020E61000006891ED7C3F2D54C052B81E85EBB14140	KJQF 	Concord Regional Airport-Concord, NC                                  	35.39	-80.707	NC 
0101000020E61000008B6CE7FBA90954C04C37894160B54140	KVUJ 	Stanly County Airport-Albemarle, NC                                   	35.417	-80.151	NC 
0101000020E6100000A4703D0AD77B54C0105839B4C8B64140	KFQD 	Rutherford County Airport-Rutherfordton, NC                           	35.428	-81.935	NC 
0101000020E6100000DF4F8D976EA254C09EEFA7C64BB74140	KAVL 	Asheville Regional Airport-Fletcher, NC                               	35.432	-82.538	NC 
0101000020E6100000F6285C8FC27D53C07B14AE47E1BA4140	KGWW 	Goldsboro-Wayne Municipal Airport-Goldsboro, NC                       	35.46	-77.965	NC 
0101000020E6100000FCA9F1D24D4A54C0E7FBA9F1D2BD4140	KIPJ 	Lincolnton-Lincoln County Regional Airport-Lincolnton, NC             	35.483	-81.161	NC 
0101000020E6100000295C8FC2F59853C06891ED7C3FC54140	KJNX 	Johnston Regional Airport-Smithfield, NC                              	35.541	-78.39	NC 
0101000020E610000033333333334353C0295C8FC2F5C84140	KOCW 	Washington-Warren Airport-Washington, NC                              	35.57	-77.05	NC 
0101000020E61000005839B4C876C653C0D122DBF97ECA4140	KTTA 	Sanford-Lee County Regional Airport-Sanford, NC                       	35.582	-79.101	NC 
0101000020E61000008D976E12835853C01B2FDD2406D14140	KPGV 	Pitt-Greenville Airport-Greenville, NC                                	35.633	-77.383	NC 
0101000020E6100000E17A14AE472154C0A69BC420B0D24140	KRUQ 	Mid-Carolina Regional Airport-Salisbury, NC                           	35.646	-80.52	NC 
0101000020E6100000E17A14AE47F953C0C1CAA145B6D34140	KHBI 	Asheboro Regional Airport-Asheboro, NC                                	35.654	-79.895	NC 
0101000020E6100000C74B378941E053C0273108AC1CDA4140	K5W8 	Siler City, NC                                                        	35.704	-79.504	NC 
0101000020E61000009CC420B0725854C0E5D022DBF9DE4140	KHKY 	Hickory Regional Airport-Hickory, NC                                  	35.742	-81.382	NC 
0101000020E61000006891ED7C3F3D54C052B81E85EBE14140	KSVH 	Statesville Regional Airport-Statesville, NC                          	35.765	-80.957	NC 
0101000020E6100000FA7E6ABC741354C08716D9CEF7E34140	KEXX 	Davidson County Airport-Lexington, NC                                 	35.781	-80.304	NC 
0101000020E6100000C976BE9F1A6754C00C022B8716E94140	KMRN 	Foothills Regional Airport-Morganton, NC                              	35.821	-81.611	NC 
0101000020E6100000FED478E9267953C03D0AD7A370ED4140	KRWI 	Rocky Mount-Wilson Regional Airport-Rocky Mount, NC                   	35.855	-77.893	NC 
0101000020E6100000355EBA490CB253C01904560E2DF24140	KRDU 	Raleigh-Durham International Airport-Raleigh, NC                      	35.892	-78.782	NC 
0101000020E6100000CDCCCCCCCCEC52C04C37894160F54140	KMQI 	Dare County Regional Airport-Manteo, NC                               	35.917	-75.7	NC 
0101000020E61000006ABC749318C453C08195438B6CF74140	KIGX 	Horace Williams Airport-Chapel Hill, NC                               	35.933	-79.064	NC 
0101000020E61000005EBA490C026353C00E2DB29DEFF74140	KETC 	Tarboro-Edgecombe Airport-Tarboro, NC                                 	35.937	-77.547	NC 
0101000020E61000006DE7FBA9F1EA52C0FCA9F1D24D024240	KFFA 	First Flight Airport-Kill Devil Hills, NC                             	36.018	-75.671	NC 
0101000020E610000085EB51B81E9553C06DE7FBA9F1024240	KLHZ 	Triangle North Executive Airport-Louisburg, NC                        	36.023	-78.33	NC 
0101000020E61000003F355EBA492453C0DD24068195034240	KEDE 	Northeastern Regional Airport-Edenton, NC                             	36.028	-76.567	NC 
0101000020E61000004A0C022B87DE53C0BC74931804064240	KBUY 	Burlington-Alamance Regional Airport-Burlington, NC                   	36.047	-79.477	NC 
0101000020E61000003108AC1C5AFC53C023DBF97E6A0C4240	KGSO 	Piedmont Triad International Airport-Greensboro, NC                   	36.097	-79.943	NC 
0101000020E610000091ED7C3F350E54C0FED478E926114240	KINT 	Smith Reynolds Airport-Winston-Salem, NC                              	36.134	-80.222	NC 
0101000020E61000009A999999996954C09A99999999194240	KTNB 	Boone Watauga Ct, NC                                                  	36.2	-81.65	NC 
0101000020E610000083C0CAA1454654C0068195438B1C4240	KUKF 	Wilkes County Airport-North Wilkesboro, NC                            	36.223	-81.098	NC 
0101000020E610000033333333330B53C0C520B07268214240	KECG 	Elizabeth City Regional Airport / Elizabeth City, NC                  	36.261	-76.175	NC 
0101000020E6100000E5D022DBF9BE53C014AE47E17A244240	KTDF 	Person County Airport-Roxboro, NC                                     	36.285	-78.984	NC 
0101000020E61000006DE7FBA9F14A53C0A01A2FDD24264240	KASJ 	Tri-County Airport-Ahoskie, NC                                        	36.298	-77.171	NC 
0101000020E6100000713D0AD7A36853C00AD7A3703D2A4240	KIXA 	Halifax/Northampton, NC                                               	36.33	-77.635	NC 
0101000020E610000060E5D022DBA153C091ED7C3F352E4240	KHNZ 	Henderson-Oxford Airport-Oxford, NC                                   	36.361	-78.529	NC 
0101000020E61000001B2FDD24060153C0508D976E12334240	KONX 	Currituck County Regional Airport-Currituck, NC                       	36.399	-76.016	NC 
0101000020E6100000894160E5D05A54C09EEFA7C64B374240	KGEV 	Ashe County Airport-Jefferson, NC                                     	36.432	-81.419	NC 
0101000020E61000005839B4C876F653C00E2DB29DEF374240	KSIF 	Rockingham County Nc Shiloh Airport-Stoneville, NC                    	36.437	-79.851	NC 
0101000020E61000003D0AD7A3706D53C0D578E92631384240	KRZZ 	Halifax County Airport-Roanoke Rapids, NC                             	36.439	-77.71	NC 
0101000020E610000008AC1C5A642354C07B14AE47E13A4240	KMWK 	Mount Airy/Surry County Airport-Mount Airy, NC                        	36.46	-80.553	NC 
0101000020E610000033C4B12E6E4B53C0AED85F764FEE4140	KMCZ 	Martin County Airport-Williamston, NC                                 	35.8618	-77.1786	NC 
0101000020E6100000B8AF03E78C3053C039B4C876BEE74140	KPMZ 	Plymouth Municipal Airport-Plymouth, NC                               	35.8105	-76.7586	NC 
0101000020E6100000E6AE25E4833254C05DFE43FAED234240	KZEF 	Elkin Municipal Airport-Elkin, NC                                     	36.2807	-80.7893	NC 
0101000020E61000000B98C0ADBB9B53C07D96E7C1DDFD4040	KLTX 	Shallotte-Wilm., NC                                                   	33.98333	-78.43333	NC 
0101000020E610000000000000008053C07D96E7C1DD5D4140	KACZ 	Henderson Field-Wallace, NC                                           	34.73333	-78	NC 
0101000020E6100000C2340C1F11F153C0B0C91AF510714140	K45J 	Rockingham, NC                                                        	34.88333	-79.76667	NC 
0101000020E6100000CDCCCCCCCCEC52C03333333333934140	K44W 	Diamond Shores, NC                                                    	35.15	-75.7	NC 
0101000020E61000006666666666F654C01630815B77974140	K6A3 	Andrews, NC                                                           	35.18333	-83.85	NC 
0101000020E610000072FE261422E252C08369183E22A24140	KHAT 	Cape Hatteras, NC                                                     	35.26667	-75.53333	NC 
0101000020E6100000CDCCCCCCCCCC54C0EACF7EA488A84140	K24A 	Sylva, NC                                                             	35.31667	-83.2	NC 
0101000020E61000008E01D9EBDDAD53C05036E50AEFAE4140	K37W 	Erwin-Harnett Co, NC                                                  	35.36667	-78.71667	NC 
0101000020E6100000CDCCCCCCCCFC52C06666666666C64140	K7W6 	Engelhard/Hyde Connty, NC                                             	35.55	-75.95	NC 
0101000020E610000066666666660654C03333333333D34140	KW44 	Asheboro, NC                                                          	35.65	-80.1	NC 
0101000020E61000003ECBF3E0EE9E53C0B79C4B7155D54140	KRAX 	Clayton-RDU 88D, NC                                                   	35.66667	-78.48333	NC 
0101000020E61000009A99999999F952C01630815B77D74140	K2DP 	Dare County Gunnery Range, NC                                         	35.68333	-75.9	NC 
0101000020E61000000000000000E053C09A99999999D94140	KSCR 	Siler City Municipal Airport-Siler City, NC                           	35.7	-79.5	NC 
0101000020E610000072FE261422B253C05036E50AEFEE4140	KRAH 	Raleigh-WFO, NC                                                       	35.86667	-78.78333	NC 
0101000020E6100000F5673F5244B454C03333333333F34140	KHSS 	Hot Springs, NC                                                       	35.9	-82.81667	NC 
0101000020E6100000C2340C1F110153C03333333333334240	K9W7 	Currituck, NC                                                         	36.4	-76.01667	NC 
0101000020E61000006666666666F653C01630815B77374240	K78N 	Reidsville, NC                                                        	36.43333	-79.85	NC 
0101000020E610000096438B6CE73358C06666666666764740	KFAR 	Hector International Airport-Fargo, ND                                	46.925	-96.811	ND 
0101000020E61000003333333333535EC07F6ABC7493484340	KMHR 	Jaffrey Airport-Silver Ranch, Jaffrey, NH                             	38.567	-121.3	NH 
0101000020E61000006B9A779CA21752C0F2B0506B9AAF4540	KCNH 	Claremont Municipal Airport-Claremont, NH                             	43.3719	-72.3693	NH 
0101000020E61000003480B74082CA51C0CC7F48BF7D654640	KERR 	Errol Airport-Errol, NH                                               	44.7929	-71.1642	NH 
0101000020E610000094F6065F98E051C0C364AA60549A4540	KOCN 	Concord Municipal Airport-Concord, NH                                 	43.2057	-71.5093	NH 
0101000020E61000003ECBF3E0EEDE51C07D96E7C1DD5D4540	KZBW 	Nashua-ARTCC-Bos, NH                                                  	42.73333	-71.48333	NH 
0101000020E61000005BCEA5B8AABA51C0E3FC4D2844A44540	K6B1 	Rochester, NH                                                         	43.28333	-70.91667	NH 
0101000020E6100000289B728577F751C08369183E22E24540	KNGR 	North Groton, NH                                                      	43.76667	-71.86667	NH 
0101000020E61000003333333333D351C03333333333F34540	KTMW 	Tamworth, NH                                                          	43.9	-71.3	NH 
0101000020E6100000F5673F5244D451C09A99999999794640	K8B2 	Diamond Lake, NH                                                      	44.95	-71.31667	NH 
0101000020E61000000C022B8716E151C04E62105839644540	KASH 	Nashua Airport (Boire Field)-Nashua, NH                               	42.783	-71.517	NH 
0101000020E6100000C74B3789410052C0D7A3703D0A674540	KAFN 	Jaffrey Airport-Silver Ranch Airpark-Jaffrey, NH                      	42.805	-72.004	NH 
0101000020E61000000C022B87161152C03333333333734540	KEEN 	Dillant-Hopkins Airport-Keene, NH                                     	42.9	-72.267	NH 
0101000020E610000096438B6CE7DB51C0D7A3703D0A774540	KMHT 	Manchester-Boston Regional Airport-Manchester, NH                     	42.93	-71.436	NH 
0101000020E61000003F355EBA49B451C0B4C876BE9F8A4540	KPSM 	Portsmouth International Airport At Pease-Portsmouth, NH              	43.083	-70.817	NH 
0101000020E6100000D578E92631E051C00AD7A3703D9A4540	KCON 	Concord Municipal Airport-Concord, NH                                 	43.205	-71.503	NH 
0101000020E61000005EBA490C02BB51C0DD24068195A34540	KDAW 	Skyhaven Airport-Rochester, NH                                        	43.278	-70.922	NH 
0101000020E6100000C1CAA145B6DB51C07F6ABC7493C84540	KLCI 	Laconia Municipal Airport-Laconia, NH                                 	43.567	-71.433	NH 
0101000020E6100000EC51B81E851352C0E3A59BC420D04540	KLEB 	Lebanon Municipal Airport-West Lebanon, NH                            	43.626	-72.305	NH 
0101000020E6100000C74B378941F051C0C1CAA145B6E34540	K1P1 	Plymouth Muni, NH                                                     	43.779	-71.754	NH 
0101000020E610000008AC1C5A64D351C0C3F5285C8F224640	KMWN 	Mount Washington, NH                                                  	44.27	-71.303	NH 
0101000020E61000007B14AE47E1E251C0C976BE9F1A2F4640	KHIE 	Mount Washington Regional Airport-Whitefield, NH                      	44.368	-71.545	NH 
0101000020E6100000FA7E6ABC74CB51C07D3F355EBA494640	KBML 	Berlin Regional Airport-Berlin, NH                                    	44.576	-71.179	NH 
0101000020E6100000273108AC1CBA52C01B2FDD2406814340	KWWD 	Cape Mayairport-Wildwood, NJ                                          	39.008	-74.908	NJ 
0101000020E6100000A245B6F3FDC452C0022B8716D9AE4340	KMIV 	Millville Municipal Airport-Millville, NJ                             	39.366	-75.078	NJ 
0101000020E61000003F355EBA49A452C060E5D022DBB94340	KACY 	Atlantic City International Airport-Atlantic City, NJ                 	39.452	-74.567	NJ 
0101000020E6100000A69BC420B09252C0105839B4C8F64340	KMJX 	Robert J. Miller Air Park-Toms River, NJ                              	39.928	-74.292	NJ 
0101000020E6100000E7FBA9F1D2B552C09CC420B072F84340	KVAY 	South Jersey Regional Airport-Mount Holly, NJ                         	39.941	-74.841	NJ 
0101000020E61000006666666666A652C01904560E2D024440	KWRI 	Mcguire Air Force Base-Wrightstown, NJ                                	40.017	-74.6	NJ 
0101000020E610000066666666669652C04E62105839044440	KNEL 	Naval Air Engineering Station-Lakehurst, NJ                           	40.033	-74.35	NJ 
0101000020E6100000736891ED7C8752C08195438B6C174440	KBLM 	Monmouth Executive Airport-Belmar / Farmingdale, NJ                   	40.183	-74.117	NJ 
0101000020E61000004E62105839B452C0FA7E6ABC74234440	KTTN 	Trenton-Mercer Airport-Trenton, NJ                                    	40.277	-74.816	NJ 
0101000020E610000000000000009052C0E5D022DBF94E4440	KLDJ 	Linden Airport-Linden, NJ                                             	40.617	-74.25	NJ 
0101000020E6100000894160E5D0AA52C01D5A643BDF4F4440	KSMQ 	Somerset Airport-Somerville, NJ                                       	40.624	-74.669	NJ 
0101000020E61000003333333333B352C0CDCCCCCCCC8C4340	KSIE 	Sea Isle VOR, NJ                                                      	39.1	-74.8	NJ 
0101000020E61000003333333333B352C01D03B2D7BB9B4340	KOBI 	Woodbine Municipal Airport-Woodbine, NJ                               	39.21667	-74.8	NJ 
0101000020E61000006666666666A652C08369183E22A24340	K26N 	Ocean City, NJ                                                        	39.26667	-74.6	NJ 
0101000020E61000000000000000B052C0B79C4B7155D54340	KN81 	Hammonton, NJ                                                         	39.66667	-74.75	NJ 
0101000020E610000072FE261422C252C09A99999999D94340	K17N 	Cross Keys, NJ                                                        	39.7	-75.03333	NJ 
0101000020E61000000B98C0ADBB9B52C0EACF7EA488E84340	KCYN 	Coyle VOR, NJ                                                         	39.81667	-74.43333	NJ 
0101000020E61000003333333333B352C01630815B77F74340	KN14 	Lumberton, NJ                                                         	39.93333	-74.8	NJ 
0101000020E61000005BCEA5B8AA9A52C09A99999999F94340	KDIX 	Manchester 88D, NJ                                                    	39.95	-74.41667	NJ 
0101000020E6100000F5673F5244B452C08369183E22024440	KPHI 	Mt Holly-PHL-WFO, NJ                                                  	40.01667	-74.81667	NJ 
0101000020E61000000B98C0ADBB8B52C0EACF7EA488084440	KN12 	Lakewood, NJ                                                          	40.06667	-74.18333	NJ 
0101000020E61000006666666666A652C01D03B2D7BB1B4440	KN87 	Robbinsville, NJ                                                      	40.21667	-74.6	NJ 
0101000020E610000066666666669652C04963B48EAA2A4440	K3N6 	Old Bridge, NJ                                                        	40.33333	-74.35	NJ 
0101000020E61000005BCEA5B8AAAA52C03333333333334440	K39N 	Princeton, NJ                                                         	40.4	-74.66667	NJ 
0101000020E6100000C2340C1F118152C01D03B2D7BB3B4440	K56N 	Sandy Hook CGS, NJ                                                    	40.46667	-74.01667	NJ 
0101000020E61000000B98C0ADBB9B52C07D96E7C1DD3D4440	KNBK 	New Brunswick, NJ                                                     	40.48333	-74.43333	NJ 
0101000020E61000006666666666A652C08369183E22424440	K47N 	Manville, NJ                                                          	40.51667	-74.6	NJ 
0101000020E61000003ECBF3E0EEBE52C0EACF7EA488484440	KN40 	Pittstown, NJ                                                         	40.56667	-74.98333	NJ 
0101000020E6100000C2340C1F11C152C04963B48EAA4A4440	KN85 	Alexandria, NJ                                                        	40.58333	-75.01667	NJ 
0101000020E61000005BCEA5B8AAAA52C05036E50AEF4E4440	KN52 	Somerville, NJ                                                        	40.61667	-74.66667	NJ 
0101000020E61000005BCEA5B8AAAA52C05036E50AEF4E4440	KSJB 	Somerville, NJ                                                        	40.61667	-74.66667	NJ 
0101000020E6100000F5673F52449452C09A99999999794440	KN07 	Lincoln Park, NJ                                                      	40.95	-74.31667	NJ 
0101000020E61000000000000000C052C01D03B2D7BB7B4440	K1N7 	Blairstown, NJ                                                        	40.96667	-75	NJ 
0101000020E610000072FE261422A252C0EACF7EA488884440	KSAX 	Sparta VOR, NJ                                                        	41.06667	-74.53333	NJ 
0101000020E610000066666666669652C0B0C91AF510914440	K4N1 	West Milford, NJ                                                      	41.13333	-74.35	NJ 
0101000020E61000006666666666A652C09A99999999994440	KN63 	Sussex, NJ                                                            	41.2	-74.6	NJ 
0101000020E6100000894160E5D08A52C08195438B6C574440	KEWR 	Newark Liberty International Airport, NJ                              	40.683	-74.169	NJ 
0101000020E6100000A69BC420B09A52C06666666666664440	KMMU 	Morristown Municipal Airport-Morristown, NJ                           	40.8	-74.417	NJ 
0101000020E610000096438B6CE78352C0CDCCCCCCCC6C4440	KTEB 	Teterboro Airport-Teterboro, NJ                                       	40.85	-74.061	NJ 
0101000020E6100000273108AC1C9252C0E3A59BC420704440	KCDW 	Essex County Airport (Caldwell Airport)-Caldwell, NJ                  	40.876	-74.283	NJ 
0101000020E6100000C976BE9F1AAF52C0FED478E926814440	K12N 	Andover, NJ                                                           	41.009	-74.736	NJ 
0101000020E6100000F2D24D6210A852C0B6F3FDD478994440	KFWN 	Sussex Airport-Sussex, NJ                                             	41.199	-74.626	NJ 
0101000020E610000004560E2DB2855AC0E5D022DBF9CE4140	KSAF 	Santa Fe Regional Airport-Santa Fe, NM                                	35.617	-106.089	NM 
0101000020E610000037894160E57052C06F1283C0CA514440	KJFK 	John F. Kennedy International Airport, NY                             	40.639	-73.764	NY 
0101000020E610000057EC2FBB27A753C0A01A2FDD24764540	KBQR 	Buffalo-Lancaster Regional Airport-Lancaster, NY                      	42.923	-78.6118	NY 
0101000020E61000009CC420B0728052C0713D0AD7A3604440	KJRA 	West 30Th Street Heliport-Manhattan, NY                               	40.755	-74.007	NY 
0101000020E6100000F6285C8FC27D52C0355EBA490C224640	KLKP 	Lake Placid Airport-Lake Placid, NY                                   	44.266	-73.965	NY 
0101000020E61000008D976E12833853C088855AD3BC5B4540	KSSN 	Seneca Army Airfield-Romulus, NY                                      	42.7167	-76.883	NY 
0101000020E61000000B98C0ADBB4B52C09A99999999394440	KN28 	Ambrose, NY                                                           	40.45	-73.18333	NY 
0101000020E610000066666666664652C06666666666664440	KZNY 	New York-ARTCC, NY                                                    	40.8	-73.1	NY 
0101000020E6100000289B7285773752C05036E50AEF6E4440	KOKX 	Brookhaven-Upton, NY                                                  	40.86667	-72.86667	NY 
0101000020E61000003ECBF3E0EE6E52C0E3FC4D2844C44440	KSTM 	Stormville, NY                                                        	41.53333	-73.73333	NY 
0101000020E610000066666666669652C01D03B2D7BBFB4440	KSLD 	Slide Mountain, NY                                                    	41.96667	-74.35	NY 
0101000020E6100000F5673F5244D452C0EACF7EA488084540	KHNK 	Hancock VOR, NY                                                       	42.06667	-75.31667	NY 
0101000020E610000066666666660653C04963B48EAA0A4540	KCZG 	Tri-Cities Airport-Endicott, NY                                       	42.08333	-76.1	NY 
0101000020E61000008E01D9EBDD6D52C0E3FC4D2844244540	K1B1 	Hudson-Columbia, NY                                                   	42.28333	-73.71667	NY 
0101000020E61000005BCEA5B8AADA52C06666666666264540	KN23 	Sidney, NY                                                            	42.3	-75.41667	NY 
0101000020E61000000B98C0ADBB6B53C0B0C91AF510314540	K4G6 	Hornell, NY                                                           	42.38333	-77.68333	NY 
0101000020E61000009A99999999C952C0B0C91AF510314540	KOZX 	Oneonta, NY                                                           	42.38333	-75.15	NY 
0101000020E6100000C2340C1F11E152C0EACF7EA488484540	KOIC 	Lt. Warren Eaton Airport-Norwich, NY                                  	42.56667	-75.51667	NY 
0101000020E6100000F5673F52448452C04963B48EAA4A4540	KENX 	Albany-E. Berne, NY                                                   	42.58333	-74.06667	NY 
0101000020E61000008E01D9EBDD0D53C0CDCCCCCCCC4C4540	KN03 	Cortland County/Chase, NY                                             	42.6	-76.21667	NY 
0101000020E61000000B98C0ADBBAB53C0B79C4B7155554540	KCOL 	Colden, NY                                                            	42.66667	-78.68333	NY 
0101000020E6100000CDCCCCCCCC8C53C01630815B77574540	KWRS 	Warsaw, NY                                                            	42.68333	-78.2	NY 
0101000020E61000005BCEA5B8AABA52C09A99999999594540	KCOO 	Cooperstown, NY                                                       	42.7	-74.91667	NY 
0101000020E610000033333333337352C00000000000604540	KALY 	Albany-WFO, NY                                                        	42.75	-73.8	NY 
0101000020E6100000F5673F5244E452C0CDCCCCCCCC6C4540	KVGC 	Hamilton Municipal, NY                                                	42.85	-75.56667	NY 
0101000020E6100000F5673F52445453C03333333333734540	KD38 	Canandaigua, NY                                                       	42.9	-77.31667	NY 
0101000020E61000000B98C0ADBB1B53C0B79C4B7155754540	K6B9 	Skaneateles Aero Drome Airport Skaneateles, NY                        	42.91667	-76.43333	NY 
0101000020E6100000A5315A47559552C07D96E7C1DD7D4540	KNY0 	Johnstown/Fulton Cnt, NY                                              	42.98333	-74.33333	NY 
0101000020E61000005BCEA5B8AA8A53C0E3FC4D2844844540	KGVQ 	Genesee County Airport-Batavia, NY                                    	43.03333	-78.16667	NY 
0101000020E6100000289B7285777752C06666666666864540	K5B2 	Saratoga Springs, NY                                                  	43.05	-73.86667	NY 
0101000020E6100000289B7285774753C07D96E7C1DD9D4540	KSDC 	Williamson-Sodus Airport-Williamson/Sodus, NY                         	43.23333	-77.11667	NY 
0101000020E6100000D8648D7A881853C0CDCCCCCCCCAC4540	KN00 	Oswego County Airport, Fulton, NY                                     	43.35	-76.38333	NY 
0101000020E61000006666666666D652C09A99999999B94540	K1B8 	Boonville, NY                                                         	43.45	-75.35	NY 
0101000020E610000072FE261422A252C09A99999999B94540	KPIC 	Piseco Lake, NY                                                       	43.45	-74.53333	NY 
0101000020E6100000CDCCCCCCCCDC52C01D03B2D7BBBB4540	KRMX 	Griffiss AFB,-88D, NY                                                 	43.46667	-75.45	NY 
0101000020E61000000B98C0ADBBEB52C00000000000E04540	KTYX 	Fort Drum,-88D, NY                                                    	43.75	-75.68333	NY 
0101000020E6100000CDCCCCCCCCBC52C01630815B77574640	KPTD 	Potsdam Municipal Airport-Potsdam, NY                                 	44.68333	-74.95	NY 
0101000020E610000033333333339352C0CDCCCCCCCC6C4640	KMAL 	Malone-Dufort Airport-Malone, NY                                      	44.85	-74.3	NY 
0101000020E610000033333333337352C03333333333734640	KELB 	Ellenburg Depot, NY                                                   	44.9	-73.8	NY 
0101000020E61000007F6ABC74938052C07D3F355EBA594440	KJRB 	Downtown Manhattan Heliport-Manhattan, NY                             	40.701	-74.009	NY 
0101000020E6100000A69BC420B05A52C0CBA145B6F35D4440	KFRG 	Republic Airport-East Farmingdale, NY                                 	40.734	-73.417	NY 
0101000020E6100000B81E85EB517852C0C1CAA145B6634440	KLGA 	La Guardia International Airport-New York, NY                         	40.779	-73.88	NY 
0101000020E6100000BC749318047E52C0C1CAA145B6634440	KNYC 	New York/Central Park, NY                                             	40.779	-73.969	NY 
0101000020E61000004A0C022B874652C01283C0CAA1654440	KISP 	Long Island Macarthur Airport-Islip, NY                               	40.794	-73.102	NY 
0101000020E6100000560E2DB29D3752C0F0A7C64B37694440	KHWV 	Brookhaven Airport-Shirley, NY                                        	40.822	-72.869	NY 
0101000020E61000009CC420B0722852C079E92631086C4440	KFOK 	Francis S. Gabreski Airport-Westhampton Beach, NY                     	40.844	-72.632	NY 
0101000020E6100000E3A59BC4201052C07B14AE47E17A4440	KHTO 	East Hampton Airport-East Hampton, NY                                 	40.96	-72.252	NY 
0101000020E610000085EB51B81E6D52C00E2DB29DEF874440	KHPN 	Westchester County Airport-White Plains, NY                           	41.062	-73.705	NY 
0101000020E6100000508D976E12FB51C0D34D621058894440	KMTP 	Montauk Airport-Montauk, NY                                           	41.073	-71.923	NY 
0101000020E610000066666666668652C00000000000C04440	KSWF 	Stewart International Airport-Newburgh, NY                            	41.5	-74.1	NY 
0101000020E6100000295C8FC2F59052C0FED478E926C14440	KMGJ 	Orange County Airport-Montgomery, NY                                  	41.509	-74.265	NY 
0101000020E61000009CC420B0727852C0E3A59BC420D04440	KPOU 	Hudson Valley Regional Airport-Poughkeepsie, NY                       	41.626	-73.882	NY 
0101000020E61000007B14AE47E1B252C07D3F355EBAD94440	KMSV 	Sullivan County International Airport-Monticello, NY                  	41.701	-74.795	NY 
0101000020E6100000736891ED7C7F53C0CBA145B6F30D4540	KELZ 	Wellsville Municipal Airport-Wellsville, NY                           	42.109	-77.992	NY 
0101000020E61000000000000000D053C03333333333134540	KJHW 	Chautauqua County-Jamestown Airport-Jamestown, NY                     	42.15	-79.25	NY 
0101000020E61000000C022B87163953C03108AC1C5A144540	KELM 	Elmira/Corning Regional Airport-Big Flats, NY                         	42.159	-76.892	NY 
0101000020E61000001F85EB51B8FE52C0D122DBF97E1A4540	KBGM 	Greater Binghamton Airport-Binghamton, NY                             	42.207	-75.98	NY 
0101000020E610000039B4C876BE9753C0022B8716D91E4540	KOLE 	Cattaraugus County-Olean Airport-Olean, NY                            	42.241	-78.371	NY 
0101000020E6100000D9CEF753E31D53C0E7FBA9F1D23D4540	KITH 	Ithaca Tompkins Regional Airport-Ithaca, NY                           	42.483	-76.467	NY 
0101000020E6100000C520B07268D153C0C976BE9F1A3F4540	KDKK 	Chautauqua County/Dunkirk Airport-Dunkirk, NY                         	42.493	-79.272	NY 
0101000020E61000001283C0CAA16D53C00C022B8716494540	KDSV 	Dansville Municipal Airport-Dansville, NY                             	42.571	-77.713	NY 
0101000020E6100000DD240681954353C0FCA9F1D24D524540	KPEO 	Penn Yan Airport (Yates County Airport)-Penn Yan, NY                  	42.643	-77.056	NY 
0101000020E61000004260E5D0227352C0560E2DB29D5F4540	KALB 	Albany International Airport-Albany, NY                               	42.747	-73.799	NY 
0101000020E6100000CDCCCCCCCC7C52C0CDCCCCCCCC6C4540	KSCH 	Schenectady County Airport-Schenectady, NY                            	42.85	-73.95	NY 
0101000020E6100000C976BE9F1AAF53C0B81E85EB51784540	KBUF 	Buffalo Niagara International Airport-Buffalo, NY                     	42.94	-78.736	NY 
0101000020E610000079E9263108BC53C0E7FBA9F1D28D4540	KIAG 	Niagara Falls International-Niagara Falls, NY                         	43.108	-78.938	NY 
0101000020E61000002DB29DEFA70653C091ED7C3F358E4540	KSYR 	Syracuse Hancock International Airport-Syracuse, NY                   	43.111	-76.104	NY 
0101000020E610000033333333336B53C0E5D022DBF98E4540	KROC 	Greater Rochester International Airport-Rochester, NY                 	43.117	-77.675	NY 
0101000020E61000007F6ABC7493D852C0C3F5285C8F924540	KUCA 	Oneida County Airport-Utica, NY                                       	43.145	-75.384	NY 
0101000020E6100000355EBA490CDA52C0CBA145B6F39D4540	KRME 	Griffiss International Airport-Rome, NY                               	43.234	-75.407	NY 
0101000020E6100000736891ED7C6752C0CDCCCCCCCCAC4540	KGFL 	Floyd Bennett Memorial Airport-Glens Falls, NY                        	43.35	-73.617	NY 
0101000020E6100000713D0AD7A31853C0CDCCCCCCCCAC4540	KFZY 	Oswego County Airport-Fulton, NY                                      	43.35	-76.385	NY 
0101000020E61000008B6CE7FBA90153C03BDF4F8D97FE4540	KART 	Watertown International Airport-Watertown, NY                         	43.989	-76.026	NY 
0101000020E6100000F4FDD478E9EE52C06666666666064640	KGTB 	Wheeler-Sack Army Airfield-Fort Drum, NY                              	44.05	-75.733	NY 
0101000020E61000006891ED7C3F8D52C0E17A14AE47314640	KSLK 	Adirondack Regional Airport-Saranac Lake, NY                          	44.385	-74.207	NY 
0101000020E6100000CBA145B6F35D52C017D9CEF753534640	KPBG 	Plattsburgh International Airport-Plattsburgh, NY                     	44.651	-73.468	NY 
0101000020E6100000E7FBA9F1D2DD52C09EEFA7C64B574640	KOGS 	Ogdensburg International Airport-Ogdensburg, NY                       	44.682	-75.466	NY 
0101000020E6100000B6F3FDD4786152C00E2DB29DEF574640	KPLB 	Plattsburgh, NY                                                       	44.687	-73.523	NY 
0101000020E6100000A01A2FDD24B652C02B8716D9CE774640	KMSS 	Massena International Airport-Massena, NY                             	44.936	-74.846	NY 
0101000020E61000000000000000A053C06891ED7C3F553E40	K2XG 	Gulf Of Mexico?                                                       	30.333	-78.5	FL 
0101000020E6100000A245B6F3FDA454C021B07268917D4340	KI43 	Jackson, OH                                                           	38.981	-82.578	OH 
0101000020E6100000894160E5D01A55C077BE9F1A2F8D4340	KLUK 	Cincinnati Municipal Lunken Airport Cincinnati, OH                    	39.103	-84.419	OH 
0101000020E6100000F4FDD478E98E54C0B29DEFA7C69B4340	KUNI 	Gordon K. Bush Airport-Athens / Albany, OH                            	39.217	-82.233	OH 
0101000020E61000009A999999992155C03BDF4F8D97AE4340	KHAO 	Butler County Regional Airport-Hamilton, OH                           	39.364	-84.525	OH 
0101000020E61000007D3F355EBAF154C0BA490C022BB74340	KILN 	Wilmington Air Park-Wilmington, OH                                    	39.431	-83.777	OH 
0101000020E6100000E17A14AE471955C08716D9CEF7C34340	KMWO 	Middletown/Hook Fld, OH                                               	39.531	-84.395	OH 
0101000020E61000005839B4C8760E55C079E9263108CC4340	KMGY 	Dayton Wright Brothers Airport-Dayton, OH                             	39.594	-84.226	OH 
0101000020E6100000355EBA490CAA54C054E3A59BC4E04340	KLHQ 	Fairfield County Airport-Lancaster, OH                                	39.756	-82.657	OH 
0101000020E6100000C1CAA145B6BB54C07F6ABC7493E84340	KLCK 	Rickenbacker International Airport-Columbus, OH                       	39.817	-82.933	OH 
0101000020E610000033333333330355C0B4C876BE9FEA4340	KFFO 	Wright-Patterson Air Force Base-Dayton, OH                            	39.833	-84.05	OH 
0101000020E61000005A643BDF4FF554C0B4C876BE9FEA4340	KSGH 	Springfield Municipal, OH                                             	39.833	-83.833	OH 
0101000020E61000008D976E1283C854C03333333333F34340	KTZR 	Columbus-Bolton, OH                                                   	39.9	-83.133	OH 
0101000020E6100000BC749318040E55C08716D9CEF7F34340	KDAY 	James M. Cox International Airport-Dayton, OH                         	39.906	-84.219	OH 
0101000020E61000000C022B87167954C046B6F3FDD4F84340	KZZV 	Zanesville Municipal Airport-Zanesville, OH                           	39.944	-81.892	OH 
0101000020E6100000E3A59BC420B854C0022B8716D9FE4340	KCMH 	John Glenn Columbus International Airport-Columbus, OH                	39.991	-82.877	OH 
0101000020E61000001283C0CAA19D54C06DE7FBA9F1024440	KVTA 	Newark-Heath Airport-Newark, OH                                       	40.023	-82.463	OH 
0101000020E6100000A245B6F3FDC454C0448B6CE7FB094440	KOSU 	Ohio State University Airport-Columbus, OH                            	40.078	-83.078	OH 
0101000020E6100000355EBA490C2255C0273108AC1C1A4440	KVES 	Darke County Airport-Versailles, OH                                   	40.204	-84.532	OH 
0101000020E61000004A0C022B87D654C0CDCCCCCCCC1C4440	KMRT 	Marysville, OH                                                        	40.225	-83.352	OH 
0101000020E61000008FC2F5285CC754C0A4703D0AD7234440	KDLZ 	Delaware Municipal Airport-Delaware, OH                               	40.28	-83.115	OH 
0101000020E61000000C022B8716A154C0B4C876BE9F2A4440	K4I3 	Mt Vernon-Knox, OH                                                    	40.333	-82.517	OH 
0101000020E61000003F355EBA49F454C0E5D022DBF92E4440	KEDJ 	Bellefontaine Regional Airport-Bellefontaine, OH                      	40.367	-83.817	OH 
0101000020E61000004260E5D0225B54C023DBF97E6A3C4440	KPHD 	Harry Clever Field Airport, New Philadelphia, OH                      	40.472	-81.424	OH 
0101000020E61000006ABC749318C454C0022B8716D94E4440	KMNN 	Marion, OH                                                            	40.616	-83.064	OH 
0101000020E6100000E7FBA9F1D2A554C0C58F31772DF14340	K7B4 	Miller Farm Landing Strip-Baltimore, OH                               	39.8842	-82.591	OH 
0101000020E61000004E62105839E454C0931804560E6D4340	KAMT 	Alexander Salamon Airport-West Union, OH                              	38.852	-83.566	OH 
0101000020E610000096438B6CE7BB54C0696FF085C9944340	KEOP 	Pike County Airport-Waverly, OH                                       	39.1624	-82.936	OH 
0101000020E6100000C520B07268D954C02AA913D044984440	KFZI 	Fostoria Metropolitan Airport-Fostoria, OH                            	41.1896	-83.397	OH 
0101000020E61000006666666666B654C04963B48EAA6A4340	KPMH 	Portsmouth, OH                                                        	38.83333	-82.85	OH 
0101000020E61000008E01D9EBDD0D55C04963B48EAA8A4340	KI69 	Batavia, OH                                                           	39.08333	-84.21667	OH 
0101000020E610000072FE2614223255C00000000000A04340	KI67 	Harrison, OH                                                          	39.25	-84.78333	OH 
0101000020E6100000F5673F5244F454C0B79C4B7155B54340	KTIR 	Wilmington-RFC, OH                                                    	39.41667	-83.81667	OH 
0101000020E6100000C2340C1F11C154C01630815B77B74340	KRZT 	Ross County Airport-Chillicothe, OH                                   	39.43333	-83.01667	OH 
0101000020E610000000000000001055C01D03B2D7BBBB4340	KI68 	Lebanon, OH                                                           	39.46667	-84.25	OH 
0101000020E610000072FE2614223255C00000000000C04340	KOXD 	Miami University Airport-Oxford, OH                                   	39.5	-84.78333	OH 
0101000020E61000003ECBF3E0EEBE54C08369183E22C24340	KCYO 	Pickaway County Memorial Airport-Circleville, OH                      	39.51667	-82.98333	OH 
0101000020E61000005BCEA5B8AADA54C0EACF7EA488C84340	KI23 	Washington Court, OH                                                  	39.56667	-83.41667	OH 
0101000020E61000003ECBF3E0EEFE54C09A99999999D94340	KI19 	Xenia, OH                                                             	39.7	-83.98333	OH 
0101000020E61000009A99999999A954C00000000000E04340	KI15 	Lancaster, OH                                                         	39.75	-82.65	OH 
0101000020E6100000D8648D7A88C854C03333333333F34340	K2I4 	Columbus-Bolton, OH                                                   	39.9	-83.13333	OH 
0101000020E61000008E01D9EBDDDD54C01630815B77F74340	KUYF 	London, OH                                                            	39.93333	-83.46667	OH 
0101000020E6100000A5315A47556554C01D03B2D7BBFB4340	KCDI 	Cambridge Municipal Airport-Cambridge, OH                             	39.96667	-81.58333	OH 
0101000020E61000008E01D9EBDD9D54C08369183E22024440	K2I8 	Newark, OH                                                            	40.01667	-82.46667	OH 
0101000020E6100000F5673F52443454C08369183E22024440	KAIR 	Bellaire VOR, OH                                                      	40.01667	-80.81667	OH 
0101000020E61000000000000000F054C05036E50AEF0E4440	KI74 	Urbana, OH                                                            	40.11667	-83.75	OH 
0101000020E610000072FE2614220255C0E3FC4D2844244440	KROD 	Rosewood VOR, OH                                                      	40.28333	-84.03333	OH 
0101000020E610000033333333331355C00000000000404440	KAXV 	Neil Armstrong Airport-Wapakoneta, OH                                 	40.5	-84.3	OH 
0101000020E6100000289B7285772755C05036E50AEF6E4440	KVNW 	Van Wert County, OH                                                   	40.86667	-84.61667	OH 
0101000020E61000005BCEA5B8AAEA54C08369183E22824440	KFBC 	Flag City, OH                                                         	41.01667	-83.66667	OH 
0101000020E610000000000000005054C01D03B2D7BB9B4440	K29G 	Ravena-Portage, OH                                                    	41.21667	-81.25	OH 
0101000020E6100000CDCCCCCCCC8C54C06666666666A64440	KZOB 	Cleveland-ARTCC, OH                                                   	41.3	-82.2	OH 
0101000020E61000000B98C0ADBB8B54C0CDCCCCCCCCAC4440	K22G 	Lorain-Elyria, OH                                                     	41.35	-82.18333	OH 
0101000020E6100000D8648D7A880855C05036E50AEFCE4440	KUSE 	Wauseon, OH                                                           	41.61667	-84.13333	OH 
0101000020E6100000CDCCCCCCCC2C54C0E3FC4D2844E44440	K7G2 	Ashtabula, OH                                                         	41.78333	-80.7	OH 
0101000020E6100000CDCCCCCCCC2C54C0E3FC4D2844E44440	KSHI 	Ashtabula, OH                                                         	41.78333	-80.7	OH 
0101000020E61000007D3F355EBA0155C0B4C876BE9F5A4440	KAOH 	Lima Allen County Airport-Lima, OH                                    	40.708	-84.027	OH 
0101000020E6100000FED478E926A154C0295C8FC2F5684440	KMFD 	Mansfield, OH                                                         	40.82	-82.518	OH 
0101000020E610000054E3A59BC47854C039B4C876BE6F4440	KBJJ 	Wayne County Airport-Wooster, OH                                      	40.873	-81.887	OH 
0101000020E610000023DBF97E6A5C54C02FDD240681754440	KCAK 	Akron-Canton Regional Airport-Akron, OH                               	40.918	-81.444	OH 
0101000020E6100000894160E5D0EA54C06F1283C0CA814440	KFDY 	Findlay, OH                                                           	41.014	-83.669	OH 
0101000020E6100000022B8716D9FE54C0F853E3A59B844440	KOWX 	Ottawa, OH                                                            	41.036	-83.982	OH 
0101000020E610000004560E2DB25D54C0BE9F1A2FDD844440	KAKR 	Akron Fulton International Airport-Akron, OH                          	41.038	-81.464	OH 
0101000020E610000000000000005054C0B29DEFA7C69B4440	KPOV 	Portage County Airport Ravenna, OH                                    	41.217	-81.25	OH 
0101000020E61000004260E5D0222B54C0713D0AD7A3A04440	KYNG 	Youngstown-Warren Regional Airport-Youngstown, OH                     	41.255	-80.674	OH 
0101000020E6100000FA7E6ABC741B55C02506819543AB4440	KDFI 	Defiance Memorial Airport-Defiance, OH                                	41.338	-84.429	OH 
0101000020E6100000FA7E6ABC748B54C03F355EBA49AC4440	KLPR 	Lorain County Regional Airport-Lorain, OH                             	41.346	-82.179	OH 
0101000020E61000004A0C022B877654C08716D9CEF7B34440	KCLE 	Cleveland Hopkins International Airport-Cleveland, OH                 	41.406	-81.852	OH 
0101000020E6100000560E2DB29DB754C0355EBA490CC24440	KPCW 	Port Clinton, OH                                                      	41.516	-82.869	OH 
0101000020E6100000B29DEFA7C66B54C0FCA9F1D24DC24440	KBKL 	Cleveland Burke Lakefront Airport-Cleveland, OH                       	41.518	-81.684	OH 
0101000020E61000005839B4C876DE54C0F2D24D6210C84440	KTDZ 	Toledo, OH                                                            	41.563	-83.476	OH 
0101000020E6100000F4FDD478E95E54C07F6ABC7493C84440	KCGF 	Cuyahoga County Airport-Cleveland, OH                                 	41.567	-81.483	OH 
0101000020E6100000DD24068195F354C04260E5D022CB4440	KTOL 	Toledo Express Airport-Toledo, OH                                     	41.587	-83.806	OH 
0101000020E61000009A999999995954C08195438B6CD74440	KLNN 	Lost Nation Airport-Willoughby, OH                                    	41.683	-81.4	OH 
0101000020E6100000068195438B2C54C0DD24068195E34440	KHZY 	Northeast Ohio Regional Airport Ashtabula, OH                         	41.778	-80.696	OH 
0101000020E6100000C1CAA145B6DB54C07F6ABC7493884640	KAPE 	Appleton, OH                                                          	45.067	-83.433	OH 
0101000020E610000039B4C876BEA757C0A69BC420B0824140	KRKR 	Robert S. Kerr Airport-Poteau, OK                                     	35.021	-94.621	OK 
0101000020E6100000A01A2FDD24CE5EC0AAF1D24D62104640	KEUG 	Mahlon Sweet Field-Eugene, OR                                         	44.128	-123.221	OR 
0101000020E6100000C1CAA145B65B53C0E7FBA9F1D2DD4340	KRYT 	Fountain Dale, PA                                                     	39.733	-77.433	PA 
0101000020E61000004A0C022B87CE52C039B4C876BEEF4340	KPHL 	Philadelphia International Airport-Philadelphia, PA                   	39.873	-75.227	PA 
0101000020E61000000E2DB29DEF3753C02FDD240681F54340	KTHV 	York Airport-York, PA                                                 	39.918	-76.874	PA 
0101000020E6100000736891ED7CF752C0E7FBA9F1D2FD4340	KMQS 	Chester County G. O. Carlson Airport-Coatesville, PA                  	39.983	-75.867	PA 
0101000020E610000046B6F3FDD4C052C0273108AC1C0A4440	KPNE 	Northeast Philadelphia Airport-Philadelphia, PA                       	40.079	-75.013	PA 
0101000020E61000000C022B8716D152C0CDCCCCCCCC0C4440	KLOM 	Philadelphia/Wings Field, PA                                          	40.1	-75.267	PA 
0101000020E6100000894160E5D01253C08FC2F5285C0F4440	KLNS 	Lancaster Airport-Lancaster, PA                                       	40.12	-76.294	PA 
0101000020E6100000273108AC1C1254C01B2FDD2406114440	KAFJ 	Washington County Airport-Washington, PA                              	40.133	-80.283	PA 
0101000020E6100000C520B072683153C00C022B8716194440	KMDT 	Harrisburg International Airport-Middletown, PA                       	40.196	-76.772	PA 
0101000020E61000009A99999999C952C09A99999999194440	KNXX 	Naval Air Station Joint Reserve Base Willow Grove, PA                 	40.2	-75.15	PA 
0101000020E61000005839B4C8763653C0B29DEFA7C61B4440	KCXY 	Capital City Airport-Harrisburg, PA                                   	40.217	-76.851	PA 
0101000020E6100000EC51B81E85E352C05839B4C8761E4440	KPTW 	Heritage Field-Pottstown, PA                                          	40.238	-75.555	PA 
0101000020E61000009A99999999D953C04E62105839244440	KLBE 	Arnold Palmer Regional Airport-Latrobe, PA                            	40.283	-79.4	PA 
0101000020E610000014AE47E17A9453C0D9CEF753E3254440	KAOO 	Altoona-Blair County Airport-Altoona, PA                              	40.296	-78.32	PA 
0101000020E61000004C37894160B553C09CC420B072284440	KJST 	Johnstown-Cambria County Airport-Johnstown, PA                        	40.316	-78.834	PA 
0101000020E61000001D5A643BDFC752C00AD7A3703D2A4440	KDYL 	Doylestown Airport-Doylestown, PA                                     	40.33	-75.123	PA 
0101000020E61000005EBA490C02FB53C03D0AD7A3702D4440	KAGC 	Allegheny County Airport-West Mifflin, PA                             	40.355	-79.922	PA 
0101000020E6100000D9CEF753E3FD52C0E5D022DBF92E4440	KRDG 	Reading Regional Airport-Reading, PA                                  	40.367	-75.967	PA 
0101000020E61000003F355EBA492453C08195438B6C374440	KMUI 	Muir Aaf/Indiantown, PA                                               	40.433	-76.567	PA 
0101000020E61000009CC420B072D852C048E17A14AE374440	KUKT 	Quakertown Airport-Quakertown, PA                                     	40.435	-75.382	PA 
0101000020E610000004560E2DB20D54C0AE47E17A143E4440	KPIT 	Pittsburgh International Airport-Pittsburgh, PA                       	40.485	-80.214	PA 
0101000020E6100000F4FDD478E9DE52C07F6ABC7493484440	KJVU 	Allentown Queen, PA                                                   	40.567	-75.483	PA 
0101000020E61000006666666666C653C01B2FDD2406514440	KIDI 	Indiana/Stewart Airport, PA                                           	40.633	-79.1	PA 
0101000020E61000008048BF7D1DDC53C0789CA223B99C4440	KAXQ 	Clarion County Airport-Clarion, PA                                    	41.2244	-79.4393	PA 
0101000020E6100000CCEEC9C342F553C0C286A757CA1A4440	KFWQ 	Rostraver Airport-Monongahela, PA                                     	40.2093	-79.8322	PA 
0101000020E6100000E09C11A5BDE953C0D7C0560916FB4340	KVVS 	Joseph A. Hardy Connellsville Airport-Connellsville, PA               	39.96161	-79.6522	PA 
0101000020E6100000637FD93D790854C0EC2FBB270FF34340	KWAY 	Greene County Airport-Waynesburg, PA                                  	39.8989	-80.1324	PA 
0101000020E6100000BBB88D06F0F652C083C0CAA145A64440	KWBW 	Wilkes-Barre Wyoming Valley Airport-Wilkes-Barre, PA                  	41.299	-75.8584	PA 
0101000020E6100000EC51B81E851753C0742497FF905A4440	KZER 	Schuylkill County Airport-Pottsville, PA                              	40.70755	-76.3675	PA 
0101000020E6100000289B728577F752C07D96E7C1DDFD4340	K40N 	Coatesville, PA                                                       	39.98333	-75.86667	PA 
0101000020E6100000A5315A4755E552C07D96E7C1DDFD4340	KOQN 	Brandywine Airport-West Chester, PA                                   	39.98333	-75.58333	PA 
0101000020E6100000C2340C1F11C153C0E3FC4D2844044440	K2G9 	Somerset County Airport-Somerset, PA                                  	40.03333	-79.01667	PA 
0101000020E6100000C2340C1F11A153C04963B48EAA0A4440	KHMZ 	Bedford County Airport-Bedford, PA                                    	40.08333	-78.51667	PA 
0101000020E6100000F5673F52444453C06666666666264440	KHAR 	Harrisburg, PA                                                        	40.3	-77.06667	PA 
0101000020E610000072FE261422D252C0B0C91AF510314440	KCKZ 	Perkasie/Pennridge A, PA                                              	40.38333	-75.28333	PA 
0101000020E61000008E01D9EBDD0D54C0E3FC4D2844444440	KPBZ 	Pitt-Coraopolis, PA                                                   	40.53333	-80.21667	PA 
0101000020E61000003ECBF3E0EEDE52C0EACF7EA488484440	KXLL 	Allentown Queen City Municipal Airport-Allentown, PA                  	40.56667	-75.48333	PA 
0101000020E61000000B98C0ADBBEB52C04963B48EAA4A4440	KETX 	East Texas VOR, PA                                                    	40.58333	-75.68333	PA 
0101000020E6100000C2340C1F11F152C06666666666664440	K22N 	Lehighton/Arner Mem, PA                                               	40.8	-75.76667	PA 
0101000020E61000005BCEA5B8AA0A54C06666666666664440	KPJC 	Zelienople, PA                                                        	40.8	-80.16667	PA 
0101000020E61000008E01D9EBDD0D54C04963B48EAA6A4440	KEWC 	Ellwood City VOR, PA                                                  	40.83333	-80.21667	PA 
0101000020E610000066666666667653C0CDCCCCCCCC6C4440	KCTP 	State College-WFO, PA                                                 	40.85	-77.85	PA 
0101000020E610000066666666667653C0CDCCCCCCCC6C4440	KRHA 	State Colleg-RFC, PA                                                  	40.85	-77.85	PA 
0101000020E610000000000000008053C0B79C4B7155754440	KCCX 	Rush-St College, PA                                                   	40.91667	-78	PA 
0101000020E610000000000000000053C07D96E7C1DD7D4440	KHZL 	Hazleton Regional Airport-Hazleton, PA                                	40.98333	-76	PA 
0101000020E61000005BCEA5B8AA9A53C06666666666864440	KN97 	Clearfield, PA                                                        	41.05	-78.41667	PA 
0101000020E61000005BCEA5B8AA5A53C0B0C91AF510914440	KLHV 	William T. Piper Memorial Airport-Lock Haven, PA                      	41.13333	-77.41667	PA 
0101000020E61000005BCEA5B8AA4A54C03333333333934440	K29D 	Grove City, PA                                                        	41.15	-81.16667	PA 
0101000020E61000005BCEA5B8AADA52C01630815B77974440	K7TB 	Tobyhanna, PA                                                         	41.18333	-75.41667	PA 
0101000020E61000000000000000A053C0B79C4B7155B54440	KOYM 	St. Marys Municipal Airport-Saint Marys, PA                           	41.41667	-78.5	PA 
0101000020E61000005BCEA5B8AACA52C07D96E7C1DDBD4440	KHAW 	Hawley, PA                                                            	41.48333	-75.16667	PA 
0101000020E61000008E01D9EBDD7D53C00000000000C04440	KSLT 	Slate Run, PA                                                         	41.5	-77.96667	PA 
0101000020E6100000CDCCCCCCCC0C54C05036E50AEFCE4440	K2G6 	Meadville, PA                                                         	41.61667	-80.2	PA 
0101000020E6100000CDCCCCCCCC1C53C07D96E7C1DDDD4440	KN27 	Towanda, PA                                                           	41.73333	-76.45	PA 
0101000020E61000009A999999995953C07D96E7C1DDDD4440	KN38 	Wellsboro/Grand Cany, PA                                              	41.73333	-77.4	PA 
0101000020E6100000E9263108ACDC52C03333333333534440	KABE 	Lehigh Valley International Airport-Allentown, PA                     	40.65	-75.448	PA 
0101000020E6100000E3A59BC4206853C02DB29DEFA7564440	KRVL 	Mifflin County Airport-Reedsville, PA                                 	40.677	-77.627	PA 
0101000020E61000009A999999991954C01904560E2D624440	KBVI 	Beaver County Airport-Beaver Falls, PA                                	40.767	-80.4	PA 
0101000020E6100000CDCCCCCCCCFC53C0FA7E6ABC74634440	KBTP 	Pittsburgh-Butler Regional Airport-Butler, PA                         	40.777	-79.95	PA 
0101000020E61000009EEFA7C64B3753C00C022B8716694440	KSEG 	Penn Valley Airport-Selinsgrove, PA                                   	40.821	-76.864	PA 
0101000020E610000066666666667653C0CDCCCCCCCC6C4440	KUNV 	University Park Airport-University Park, PA                           	40.85	-77.85	PA 
0101000020E61000005A643BDF4F8553C03333333333734440	KPSB 	Mid-State Regional Airport-Philipsburg, PA                            	40.9	-78.083	PA 
0101000020E6100000A69BC420B01A54C04E62105839844440	KUCP 	New Castle Municipal Airport-New Castle, PA                           	41.033	-80.417	PA 
0101000020E6100000EE7C3F355E9A53C0BC74931804864440	KFIG 	Clearfield-Lawrence Airport-Clearfield, PA                            	41.047	-78.412	PA 
0101000020E6100000C74B378941D852C06F1283C0CA914440	KMPO 	Pocono Mountains Municipal Airport-Mount Pocono, PA                   	41.139	-75.379	PA 
0101000020E6100000FED478E926B953C0F4FDD478E9964440	KDUJ 	Dubois Regional Airport-Dubois, PA                                    	41.179	-78.893	PA 
0101000020E61000005EBA490C023B53C0C976BE9F1A9F4440	KIPT 	Williamsport Regional Airport-Williamsport, PA                        	41.243	-76.922	PA 
0101000020E61000004A0C022B87EE52C0986E1283C0AA4440	KAVP 	Wilkes-Barre/Scranton International Airport-Avoca, PA                 	41.334	-75.727	PA 
0101000020E6100000736891ED7CF753C01B2FDD2406B14440	KFKL 	Venango Regional Airport-Franklin, PA                                 	41.383	-79.867	PA 
0101000020E6100000F6285C8FC20D54C0E3A59BC420D04440	KGKJ 	Port Meadville Airport-Meadville, PA                                  	41.626	-80.215	PA 
0101000020E610000062105839B4A853C0A01A2FDD24E64440	KBFD 	Bradford Regional Airport-Bradford, PA                                	41.798	-78.636	PA 
0101000020E6100000CFF753E3A50B54C00AD7A3703D0A4540	KERI 	Erie International Airport-Erie, PA                                   	42.08	-80.182	PA 
0101000020E6100000AC1C5A643BD752C0B81E85EB51584340	KGED 	Delaware Coastal Airport-Georgetown, DE                               	38.69	-75.363	DE 
0101000020E6100000D9CEF753E3DD52C01B2FDD2406914340	KDOV 	Dover Air Force Base-Dover, DE                                        	39.133	-75.467	DE 
0101000020E61000005839B4C876E652C0A01A2FDD24D64340	KILG 	Wilmington Airport-Wilmington, DE                                     	39.673	-75.601	DE 
0101000020E6100000BC0512143FEE52C0ED0DBE3099C24340	KEVY 	Summit Airport-Middletown, DE                                         	39.5203	-75.7226	DE 
0101000020E610000072FE261422D251C0E3FC4D2844C44440	K2B4 	Newport, RI                                                           	41.53333	-71.28333	RI 
0101000020E6100000C2340C1F11E151C0B79C4B7155F54440	K1B6 	Woonsocket, RI                                                        	41.91667	-71.51667	RI 
0101000020E6100000A245B6F3FDE451C02FDD240681954440	KBID 	Block Island State Airport-Block Island, RI                           	41.168	-71.578	RI 
0101000020E61000004260E5D022F351C0CDCCCCCCCCAC4440	KWST 	Westerly State Airport-Westerly, RI                                   	41.35	-71.799	RI 
0101000020E6100000273108AC1CD251C04E62105839C44440	KUUU 	Newport State Airport-Newport, RI                                     	41.533	-71.283	RI 
0101000020E6100000EE7C3F355EDA51C023DBF97E6ACC4440	KOQU 	Quonset State Airport-North Kingstown, RI                             	41.597	-71.412	RI 
0101000020E6100000C1CAA145B6DB51C0068195438BDC4440	KPVD 	T.F. Green State Airport-Providence, RI                               	41.723	-71.433	RI 
0101000020E6100000F4FDD478E9DE51C0D9CEF753E3F54440	KSFZ 	North Central State Airport-Pawtucket, RI                             	41.921	-71.483	RI 
0101000020E61000003F355EBA491C55C0713D0AD7A3D04040	KATL 	Hartsfield-Jackson Atlanta International Airport, GA                  	33.63	-84.442	GA 
0101000020E6100000CDCCCCCCCC2C54C0B29DEFA7C61B4040	KHXD 	Hilton Head Airport-Hilton Head Island, SC                            	32.217	-80.7	SC 
0101000020E61000007F6ABC74932854C0DBF97E6ABC344040	KARW 	Beaufort County Airport-Beaufort County, SC                           	32.412	-80.634	SC 
0101000020E6100000D9CEF753E32D54C0E7FBA9F1D23D4040	KNBC 	Marine Corps Air Station Beaufort-Beaufort, SC                        	32.483	-80.717	SC 
0101000020E6100000D578E926310054C07D3F355EBA594040	KJZI 	Charleston Executive Airport-Charleston, SC                           	32.701	-80.003	SC 
0101000020E6100000C1CAA145B6FB53C0CDCCCCCCCC6C4040	KCLX 	Charleston-Grays, SC                                                  	32.85	-79.933	SC 
0101000020E6100000B4C876BE9F0254C0508D976E12734040	KCHS 	Charleston International Airport-Charleston, SC                       	32.899	-80.041	SC 
0101000020E6100000273108AC1CF253C03333333333734040	KLRO 	Mount Pleasant, SC                                                    	32.9	-79.783	SC 
0101000020E61000008D976E12832854C04C37894160754040	KRBW 	Lowcountry Regional Airport-Walterboro, SC                            	32.917	-80.633	SC 
0101000020E610000060E5D022DB1154C0F2D24D6210884040	KDYB 	Summerville Airport-Summerville, SC                                   	33.063	-80.279	SC 
0101000020E6100000273108AC1C0254C08195438B6C974040	KMKS 	Berkeley Moncks, SC                                                   	33.183	-80.033	SC 
0101000020E61000008D976E12835854C00000000000A04040	KBNL 	Barnwell Regional Airport-Barnwell, SC                                	33.25	-81.383	SC 
0101000020E61000003F355EBA49D453C07F6ABC7493A84040	KGGE 	Georgetown County Airport-Georgetown, SC                              	33.317	-79.317	SC 
0101000020E6100000F4FDD478E93654C04260E5D022BB4040	KOGB 	Orangeburg Municipal Airport-Orangeburg, SC                           	33.462	-80.858	SC 
0101000020E61000004C378941600D54C04260E5D022CB4040	KMNI 	Santee Cooper Regional Airport-Manning, SC                            	33.587	-80.209	SC 
0101000020E61000004C378941604554C01F85EB51B8CE4040	KXNO 	North Air Force Auxiliary Field-North, SC                             	33.615	-81.084	SC 
0101000020E6100000C1CAA145B66B54C03333333333D34040	KAIK 	Aiken Municipal Airport-Aiken, SC                                     	33.65	-81.683	SC 
0101000020E6100000C1CAA145B6BB53C08195438B6CD74040	KMYR 	Myrtle Beach International Airport-Myrtle Beach, SC                   	33.683	-78.933	SC 
0101000020E61000006666666666F653C0B29DEFA7C6DB4040	KCKI 	Williamsburg Regional Airport-Kingstree, SC                           	33.717	-79.85	SC 
0101000020E6100000AE47E17A14AE53C09CC420B072E84040	KCRE 	Grand Strand Airport-North Myrtle Beach, SC                           	33.816	-78.72	SC 
0101000020E61000002B8716D9CEC753C0448B6CE7FBE94040	KHYW 	Conway-Horry County Airport-Conway, SC                                	33.828	-79.122	SC 
0101000020E6100000643BDF4F8D4754C07F6ABC7493F84040	KCAE 	Columbia Metropolitan Airport-West Columbia, SC                       	33.942	-81.118	SC 
0101000020E610000033333333333354C0B29DEFA7C6FB4040	KMMT 	Mcentire Joint National Guard Base-Eastover, SC                       	33.967	-80.8	SC 
0101000020E6100000D9CEF753E31D54C0B29DEFA7C6FB4040	KSSC 	Shaw Air Force Base-Sumter, SC                                        	33.967	-80.467	SC 
0101000020E610000039B4C876BE3F54C03F355EBA49FC4040	KCUB 	Columbia Owens Downtown Airport-Columbia, SC                          	33.971	-80.996	SC 
0101000020E6100000736891ED7C1754C00000000000004140	KSMS 	Sumter Airport-Sumter, SC                                             	34	-80.367	SC 
0101000020E61000003D0AD7A370D553C0BA490C022B174140	KMAO 	Marion County Airport-Marion, SC                                      	34.181	-79.335	SC 
0101000020E6100000105839B4C8EE53C0F2D24D6210184140	KFLO 	Florence Regional Airport-Florence, SC                                	34.188	-79.731	SC 
0101000020E61000001904560E2D8A54C01D5A643BDF1F4140	KGRD 	Greenwood County Airport-Greenwood, SC                                	34.249	-82.159	SC 
0101000020E61000003F355EBA492454C04E62105839244140	KCDN 	Woodward Field (Kershaw County Airport)-Camden, SC                    	34.283	-80.567	SC 
0101000020E61000008D976E12836854C06666666666264140	KEOE 	Newberry County Airport-Newberry, SC                                  	34.3	-81.633	SC 
0101000020E6100000E5D022DBF94654C0B81E85EB51284140	KFDW 	Fairfield County Airport-Winnsboro, SC                                	34.315	-81.109	SC 
0101000020E6100000736891ED7C0754C03333333333334140	KHVS 	Hartsville Regional Airport-Hartsville, SC                            	34.4	-80.117	SC 
0101000020E6100000295C8FC2F5F853C0CFF753E3A53B4140	KUDG 	Darlington County Jetport-Darlington, SC                              	34.466	-79.89	SC 
0101000020E61000003D0AD7A370AD54C039B4C876BE3F4140	KAND 	Anderson Regional Airport-Anderson, SC                                	34.498	-82.71	SC 
0101000020E6100000CDCCCCCCCC7C54C00000000000404140	KLUX 	Laurens, SC                                                           	34.5	-81.95	SC 
0101000020E6100000F4FDD478E9EE53C0E5D022DBF94E4140	KBBP 	Marlboro County Jetport-Bennettsville, SC                             	34.617	-79.733	SC 
0101000020E610000062105839B4B854C0BC74931804564140	KCEU 	Oconee County Regional Airport-Clemson, SC                            	34.672	-82.886	SC 
0101000020E6100000CDCCCCCCCCFC53C0B29DEFA7C65B4140	KCQW 	Cheraw Municipal Airport-Cheraw, SC                                   	34.717	-79.95	SC 
0101000020E610000066666666663654C0B29DEFA7C65B4140	KLKR 	Lancaster County Airport (Mcwhirter Field)-Lancaster, SC              	34.717	-80.85	SC 
0101000020E6100000F2D24D62109854C01B2FDD2406614140	KGYH 	Donaldson Center Airport-Greenville, SC                               	34.758	-82.376	SC 
0101000020E6100000CDCCCCCCCC4C54C04E62105839644140	KDCM 	Chester Catawba, SC                                                   	34.783	-81.2	SC 
0101000020E6100000CDCCCCCCCCAC54C07F6ABC7493684140	KLQK 	Pickens County Airport-Pickens, SC                                    	34.817	-82.7	SC 
0101000020E6100000A01A2FDD249654C03F355EBA496C4140	KGMU 	Greenville Downtown Airport-Greenville, SC                            	34.846	-82.346	SC 
0101000020E61000001283C0CAA18D54C08716D9CEF7734140	KGSP 	Greenville-Spartanburg International Airport, SC                      	34.906	-82.213	SC 
0101000020E6100000CDCCCCCCCC7C54C04C37894160754140	KSPA 	Spartanburg Downtown Memorial Airport-Spartanburg, SC                 	34.917	-81.95	SC 
0101000020E6100000C1CAA145B64354C075931804567E4140	KUZA 	Rock Hill/York County Airport-Rock Hill, SC                           	34.987	-81.058	SC 
0101000020E6100000DDB5847CD0D753C0287E8CB96B394140	KDLC 	Dillon County Airport-Dillon, SC                                      	34.4486	-79.3721	SC 
0101000020E610000092CB7F48BFE153C036CD3B4ED1B94040	KPHH 	Robert F. Swinnie Airport-Andrews, SC                                 	33.4517	-79.5273	SC 
0101000020E6100000BDE3141DC91554C03A92CB7F485F4140	KPYG 	Pageland Airport-Pageland, SC                                         	34.7444	-80.3404	SC 
0101000020E6100000CDCCCCCCCC2C54C01D03B2D7BB1B4040	K49J 	Hilton Head, SC                                                       	32.21667	-80.7	SC 
0101000020E610000072FE2614220254C03333333333734040	KIGC 	Charleston Afb, SC                                                    	32.9	-80.03333	SC 
0101000020E6100000C2340C1F115154C07D96E7C1DD7D4040	KAQX 	Allendale County, SC                                                  	32.98333	-81.26667	SC 
0101000020E61000005BCEA5B8AA8A54C01D03B2D7BBDB4040	KIRQ 	Colliers VOR, SC                                                      	33.71667	-82.16667	SC 
0101000020E61000003ECBF3E0EE1E54C0CDCCCCCCCCEC4040	K2PJ 	Sumter-Poinsett, SC                                                   	33.85	-80.48333	SC 
0101000020E6100000D8648D7A889854C00000000000604140	KXAA 	Grnvl-Dnldsn, SC                                                      	34.75	-82.38333	SC 
0101000020E610000033333333334354C07D96E7C1DD7D4140	K29J 	Rock Hill, SC                                                         	34.98333	-81.05	SC 
0101000020E6100000FA7E6ABC74C359C0D9CEF753E3054640	KRAP 	Rapid City Regional Airport-Rapid City, SD                            	44.046	-103.054	SD 
0101000020E6100000C976BE9F1AFF54C062105839B4E84140	KTYS 	Mc Ghee Tyson Airport, Knoxville, TN                                  	35.818	-83.986	TN 
0101000020E61000006ABC749318AC55C0AC1C5A643B0F4240	KBNA 	Nashville Internationl Airport, TN                                    	36.119	-86.689	TN 
0101000020E610000060E5D022DB9954C0068195438B3C4240	KTRI 	Bristol/Tri-Cities Regional Airport, TN                               	36.473	-82.404	TN 
0101000020E6100000083D9B559F7355C03D0AD7A370FD4140	K0A3 	Smithville Municipal Airport-Smithville, TN                           	35.98	-85.8066	TN 
0101000020E61000003ECBF3E0EE9E55C0CDCCCCCCCC4C4240	K1M5 	Portland Municipal Airport-Portland, TN                               	36.6	-86.48333	TN 
0101000020E61000006DE7FBA9F14258C02FDD240681153C40	KRKP 	Aransas Coounty-Rockport, TX                                          	28.084	-97.046	TX 
0101000020E6100000E5D022DBF99E58C025068195438B3D40	KSAT 	San Antonio International Airport, TX                                 	29.544	-98.484	TX 
0101000020E6100000D7A3703D0AD757C07B14AE47E1FA3D40	KIAH 	George Bush International-Houston, TX                                 	29.98	-95.36	TX 
0101000020E6100000EC51B81E856B58C0022B8716D92E3E40	KAUS 	Austin/Bergstrom International Airport, TX                            	30.183	-97.68	TX 
0101000020E6100000643BDF4F8D3758C0BA490C022B574040	KRBD 	Dallas Executive Airport-Dallas, TX                                   	32.681	-96.868	TX 
0101000020E6100000F0A7C64B374158C06DE7FBA9F1724040	KDFW 	Dallas-Ft Worth International Airport, TX                             	32.898	-97.019	TX 
0101000020E61000003108AC1C5A5458C0068195438B7C4040	KAFW 	Fort Worth Alliance-Fort Worth, TX                                    	32.973	-97.318	TX 
0101000020E610000091ED7C3F35B658C0CDCCCCCCCC6C3A40	K67R 	Rio Grande City Municipal Airport Rio Grande City, TX                 	26.425	-98.847	TX 
0101000020E6100000B4C876BE9F4A58C09C33A2B437083E40	K84R 	Smithville Crawford Muniicipal-Smithville, TX                         	30.0321	-97.166	TX 
0101000020E61000003D0AD7A370D553C0D34D621058494240	KDAN 	Danville Regional Airport-Danville, VA                                	36.573	-79.335	VA 
0101000020E6100000FED478E9260154C054E3A59BC4504240	KMTV 	Blue Ridge Airport-Martinsville, VA                                   	36.631	-80.018	VA 
0101000020E6100000CBA145B6F3CD54C0C1CAA145B6534240	K0VG 	Jonesville, VA                                                        	36.654	-83.218	VA 
0101000020E6100000068195438B1453C06891ED7C3F554240	KCPK 	Chesapeake Regional Airport-Norfolk, VA                               	36.666	-76.321	VA 
0101000020E61000004A0C022B872653C09EEFA7C64B574240	KSFQ 	Suffolk Executive Airport-Suffolk, VA                                 	36.682	-76.602	VA 
0101000020E6100000273108AC1C8254C08195438B6C574240	KVJI 	Virginia Highlands Airport-Abingdon, VA                               	36.683	-82.033	VA 
0101000020E6100000F4FDD478E95E53C00E2DB29DEF574240	KEMV 	Emporia-Greensville Regional Airport-Emporia, VA                      	36.687	-77.483	VA 
0101000020E6100000FA7E6ABC748353C0F2D24D6210584240	KAVC 	Mecklenburg-Brunswick Regional Airport-South Hill, VA                 	36.688	-78.054	VA 
0101000020E610000062105839B40853C0295C8FC2F5584240	KNFE 	Naval Auxiliary Landing Field Fentress-Chesapeake, VA                 	36.695	-76.136	VA 
0101000020E61000006F1283C0CA3953C0D34D621058594240	KFKN 	Franklin Municipal-John Beverly Rose Airport-Franklin, VA             	36.698	-76.903	VA 
0101000020E6100000E9263108AC3454C0355EBA490C624240	KHLX 	Twin County Airport-Galax-Hillsville, VA                              	36.766	-80.823	VA 
0101000020E6100000894160E5D07253C06DE7FBA9F1624240	KLVL 	Lawrenceville Municipal Airport-Lawrenceville, VA                     	36.773	-77.794	VA 
0101000020E6100000CDCCCCCCCC1C53C04E62105839644240	KPVG 	Hampton Roads Executive Airport-Chesapeake, VA                        	36.783	-76.45	VA 
0101000020E6100000273108AC1C0253C07F6ABC7493684240	KNTU 	Naval Air Station Oceana-Virginia Beach, VA                           	36.817	-76.033	VA 
0101000020E610000066666666665654C0C3F5285C8F724240	KMKJ 	Mountain Empire Airport-Marion / Wytheville, VA                       	36.895	-81.35	VA 
0101000020E61000003F355EBA490C53C0DD24068195734240	KORF 	Norfolk International Airport-Norfolk, VA                             	36.903	-76.192	VA 
0101000020E6100000D122DBF97E1253C00E2DB29DEF774240	KNGU 	Naval Station Norfolk-Norfolk, VA                                     	36.937	-76.289	VA 
0101000020E6100000F2D24D62104053C0E7FBA9F1D27D4240	KAKQ 	Wakefield Municipal Airport-Wakefield, VA                             	36.983	-77.001	VA 
0101000020E610000052B81E85EBA154C05839B4C8767E4240	KLNP 	Lonesome Pine Airport-Wise, VA                                        	36.988	-82.53	VA 
0101000020E6100000508D976E127354C0D578E92631884240	K6V3 	Richlands, VA                                                         	37.064	-81.798	VA 
0101000020E610000033333333337354C07F6ABC7493884240	KJFZ 	Richlands, VA                                                         	37.067	-81.8	VA 
0101000020E6100000CDCCCCCCCC7C53C0B4C876BE9F8A4240	KBKT 	Allen C. Perkinson Airport-Blackstone, VA                             	37.083	-77.95	VA 
0101000020E6100000D7A3703D0A1753C0B4C876BE9F8A4240	KLFI 	Langley Air Force Base-Hampton, VA                                    	37.083	-76.36	VA 
0101000020E6100000643BDF4F8D1F53C037894160E5904240	KPHF 	Newport News/Williamsburg International-Newport News, VA              	37.132	-76.493	VA 
0101000020E610000066666666662653C01B2FDD2406914240	KFAF 	Joint Base Langley-Eustis-Fort Eustis, VA                             	37.133	-76.6	VA 
0101000020E6100000C1CAA145B62B54C01B2FDD2406914240	KPSK 	New River Valley Airport-Dublin, VA                                   	37.133	-80.683	VA 
0101000020E610000000000000006053C08195438B6C974240	KPTB 	Dinwiddie County Airport-Petersburg, VA                               	37.183	-77.5	VA 
0101000020E6100000273108AC1C1A54C0B4C876BE9F9A4240	KBCB 	Virginiatech/Montgomery Executive Airport-Blacksburg, VA              	37.208	-80.408	VA 
0101000020E6100000E7FBA9F1D22D53C03BDF4F8D979E4240	KJGG 	Williamsburg-Jamestown Airport-Williamsburg, VA                       	37.239	-76.716	VA 
0101000020E61000007593180456FE53C07F6ABC7493A84240	KROA 	Roanoke-Blacksburg Regional Airport-Roanoke, VA                       	37.317	-79.974	VA 
0101000020E61000006891ED7C3FCD53C00C022B8716A94240	KLYH 	Lynchburg Regional Airport-Lynchburg, VA                              	37.321	-79.207	VA 
0101000020E610000079E92631089C53C0E7FBA9F1D2AD4240	KFVX 	Farmville Regional Airport-Farmville, VA                              	37.358	-78.438	VA 
0101000020E61000000C022B87166153C03333333333B34240	KFCI 	Chesterfield County Airport-Richmond, VA                              	37.4	-77.517	VA 
0101000020E6100000E9263108AC5453C0A8C64B3789C14240	KRIC 	Richmond International Airport-Sandston, VA                           	37.512	-77.323	VA 
0101000020E6100000295C8FC2F53053C0A69BC420B0C24240	KFYJ 	Middle Peninsula Regional Airport-West Point, VA                      	37.521	-76.765	VA 
0101000020E610000062105839B4F052C0894160E5D0D24240	KMFV 	Accomack County Airport-Melfa, VA                                     	37.647	-75.761	VA 
0101000020E6100000B29DEFA7C65B53C0B4C876BE9FDA4240	KOFP 	Hanover County Municipal Airport-Richmond, VA                         	37.708	-77.434	VA 
0101000020E61000008D976E12833853C0CDCCCCCCCCEC4240	KXSA 	Tappahannock, VA                                                      	37.85	-76.883	VA 
0101000020E6100000E7FBA9F1D2DD52C00E2DB29DEFF74240	KWAL 	Wallops Flight Facility-Wallops Island, VA                            	37.937	-75.466	VA 
0101000020E61000003F355EBA49F453C09A99999999F94240	KHSP 	Ingalls Field-Hot Springs, VA                                         	37.95	-79.817	VA 
0101000020E6100000AE47E17A147E53C0E17A14AE47014340	KLKU 	Louisa County Airport (Freeman Field)-Louisa, VA                      	38.01	-77.97	VA 
0101000020E610000085EB51B81E9D53C0A8C64B3789114340	KCHO 	Charlottesville-Albemarle Airport-Charlottesville, VA                 	38.137	-78.455	VA 
0101000020E61000006DE7FBA9F18253C0560E2DB29D1F4340	KOMH 	Orange County Airport-Orange, VA                                      	38.247	-78.046	VA 
0101000020E6100000D34D621058B953C06F1283C0CA214340	KSHD 	Shenandoah Valley Regional Airport-Staunton, VA                       	38.264	-78.896	VA 
0101000020E6100000DBF97E6ABC5C53C01904560E2D224340	KEZF 	Shannon Airport-Fredericksburg, VA                                    	38.267	-77.449	VA 
0101000020E610000077BE9F1A2F5D53C06DE7FBA9F1324340	KRMN 	Stafford Regional Airport-Stafford, VA                                	38.398	-77.456	VA 
0101000020E6100000EC51B81E855353C08D976E1283404340	KNYG 	Marine Corps Air Facility Quantico-Quantico, VA                       	38.504	-77.305	VA 
0101000020E6100000E5D022DBF97653C0FA7E6ABC74434340	KCJR 	Culpeper Regional Airport-Culpeper, VA                                	38.527	-77.859	VA 
0101000020E61000002FDD2406816D53C05EBA490C024B4340	KHWY 	Warrenton-Fauquier Airport-Warrenton, VA                              	38.586	-77.711	VA 
0101000020E6100000F2D24D6210A053C04C37894160554340	KLUA 	Luray Caverns Arpt, VA                                                	38.667	-78.501	VA 
0101000020E6100000F2D24D6210A053C04C37894160554340	KW45 	Luray, VA                                                             	38.667	-78.501	VA 
0101000020E6100000C1CAA145B64B53C0B29DEFA7C65B4340	KDAA 	Davison Army Airfield-Fort Belvoir, VA                                	38.717	-77.183	VA 
0101000020E6100000295C8FC2F56053C03F355EBA495C4340	KHEF 	Manassas Regional Airport-Manassas, VA                                	38.721	-77.515	VA 
0101000020E61000000AD7A3703D4253C023DBF97E6A6C4340	KDCA 	Ronald Reagan National Airport-Arlington County, VA                   	38.847	-77.035	VA 
0101000020E610000004560E2DB25D53C09CC420B072784340	KIAD 	Washington Dulles International Airport-Dulles, VA                    	38.941	-77.464	VA 
0101000020E6100000C1CAA145B66353C0448B6CE7FB894340	KJYO 	Leesburg Executive Airport-Leesburg, VA                               	39.078	-77.558	VA 
0101000020E6100000F0A7C64B378953C0FCA9F1D24D924340	KOKV 	Winchester Regional Airport-Winchester, VA                            	39.143	-78.144	VA 
0101000020E61000003108AC1C5A5453C046B6F3FDD4084340	KAPH 	A.P. Hill Army Airfield-Fort A.P. Hill, VA                            	38.069	-77.318	VA 
0101000020E6100000645DDC46038854C092CB7F48BF9D4240	KGDY 	Grundy Municipal Airport-Grundy, VA                                   	37.2324	-82.1252	VA 
0101000020E6100000A60A4625754253C0D0B359F5B92A4340	KNDY 	Naval Surface Warfare Center Dahlgren-Dahlgren, VA                    	38.3338	-77.0384	VA 
0101000020E61000003333333333A353C0CDCCCCCCCC4C4240	KW63 	Clarksville/Lake Cou, VA                                              	36.6	-78.55	VA 
0101000020E61000006666666666B653C09A99999999594240	KW78 	South Boston/Tuck AR, VA                                              	36.7	-78.85	VA 
0101000020E61000000000000000A053C0E3FC4D2844644240	KCXE 	Chase City Municipal Airport-Chase City, VA                           	36.78333	-78.5	VA 
0101000020E61000003ECBF3E0EEBE53C09A99999999794240	KVQN 	Volens, VA                                                            	36.95	-78.98333	VA 
0101000020E61000000B98C0ADBB8B53C09A99999999794240	KW31 	Lunenburg County Airport Kenbridge, VA                                	36.95	-78.18333	VA 
0101000020E6100000C2340C1F111154C08369183E22824240	KFCX 	Roanoke-Floyd County, VA                                              	37.01667	-80.26667	VA 
0101000020E6100000C2340C1F11C153C03333333333934240	K0V4 	Brookneal/Campbell County, VA                                         	37.15	-79.01667	VA 
0101000020E610000066666666668653C01630815B77974240	KW81 	Crewe Muni, VA                                                        	37.18333	-78.1	VA 
0101000020E61000009A999999991954C09A99999999994240	KRNK 	Roanoke-Blacksbg, VA                                                  	37.2	-80.4	VA 
0101000020E6100000D8648D7A882853C06666666666A64240	KW94 	Williamsburg, VA                                                      	37.3	-76.63333	VA 
0101000020E6100000289B7285774753C00000000000C04240	KW96 	Quinton/New Kent Center, VA                                           	37.5	-77.11667	VA 
0101000020E6100000CDCCCCCCCC1C53C0CDCCCCCCCCCC4240	KW75 	Saluda/Hummel Field, VA                                               	37.6	-76.45	VA 
0101000020E61000003ECBF3E0EEFE52C0EACF7EA488E84240	KTGI 	Tangier Island Airport-Tangier, VA                                    	37.81667	-75.98333	VA 
0101000020E610000000000000007053C01D03B2D7BBFB4240	K7W4 	Bumpass/Lake Anna, VA                                                 	37.96667	-77.75	VA 
0101000020E6100000CDCCCCCCCCBC53C04963B48EAA0A4340	KW13 	Waynesboro/Eagles Nest, VA                                            	38.08333	-78.95	VA 
0101000020E61000005BCEA5B8AA8A53C03333333333134340	KGVE 	Gordonsville Municipal Airport-Gordonsville, VA                       	38.15	-78.16667	VA 
0101000020E61000008E01D9EBDDBD53C05036E50AEF2E4340	KVBW 	Bridgewater Airport-Bridgewater, VA                                   	38.36667	-78.96667	VA 
0101000020E61000008E01D9EBDD6D53C04963B48EAA4A4340	KW66 	Warrenton, VA                                                         	38.58333	-77.71667	VA 
0101000020E6100000289B7285777753C0B0C91AF510514340	KCSN 	Cassanova VOR, VA                                                     	38.63333	-77.86667	VA 
0101000020E6100000CDCCCCCCCCAC53C03333333333534340	K8W2 	New Market, VA                                                        	38.65	-78.7	VA 
0101000020E610000000000000009053C0B79C4B7155754340	KFRR 	Front Royal-Warren County Airport-Front Royal, VA                     	38.91667	-78.25	VA 
0101000020E61000003ECBF3E0EE5E53C07D96E7C1DD7D4340	KLWX 	Sterling, VA                                                          	38.98333	-77.48333	VA 
0101000020E6100000289B7285770752C05036E50AEF6E4540	KMNW 	Wilmington, VT                                                        	42.86667	-72.11667	VT 
0101000020E610000000000000005052C0B0C91AF510714540	K5B5 	Bennington, VT                                                        	42.88333	-73.25	VT 
0101000020E610000066666666663652C01630815B77774540	KWDO 	West Dover, VT                                                        	42.93333	-72.85	VT 
0101000020E610000033333333333352C05036E50AEF8E4540	K8B3 	Ball Mountain, VT                                                     	43.11667	-72.8	VT 
0101000020E610000066666666664652C07D96E7C1DDFD4540	K6B0 	Middlebury, VT                                                        	43.98333	-73.1	VT 
0101000020E61000008E01D9EBDD3D52C0EACF7EA488084640	KSOL 	South Lincoln, VT                                                     	44.06667	-72.96667	VT 
0101000020E61000005BCEA5B8AA4A52C08369183E22424640	KCXX 	Colchester,-88D, VT                                                   	44.51667	-73.16667	VT 
0101000020E6100000C2340C1F110152C0545227A089484640	K6B8 	Lyndonville, VT                                                       	44.5667	-72.01667	VT 
0101000020E61000003ECBF3E0EEFE51C03333333333534640	KVT2 	West Burke, VT                                                        	44.65	-71.98333	VT 
0101000020E610000072FE2614222252C0ACADD85F76774640	KJAY 	Jay Peak, VT                                                          	44.9333	-72.53333	VT 
0101000020E61000000000000000E051C012143FC6DC7D4640	KVT1 	Canaan, VT                                                            	44.9833	-71.5	VT 
0101000020E61000002B8716D9CE4F52C0355EBA490C724540	KDDH 	William H. Morse State Airport-Bennington, VT                         	42.891	-73.247	VT 
0101000020E6100000FED478E9262152C079E9263108AC4540	KVSF 	Hartness State Airport-Springfield, VT                                	43.344	-72.518	VT 
0101000020E6100000CDCCCCCCCC3C52C04E62105839C44540	KRUT 	Rutland-Southern Vermont Regional Airport-Rutland, VT                 	43.533	-72.95	VT 
0101000020E61000008716D9CEF72352C0273108AC1C1A4640	KMPV 	Edward F. Knapp State Airport-Barre-Montpelier, VT                    	44.204	-72.562	VT 
0101000020E6100000F0A7C64B370152C0F6285C8FC2354640	K1V4 	Saint Johnsbury St. Johnsbury, VT                                     	44.42	-72.019	VT 
0101000020E61000009A999999994952C096438B6CE73B4640	KBTV 	Burlington International Airport-Burlington, VT                       	44.468	-73.15	VT 
0101000020E61000009EEFA7C64B2752C03108AC1C5A444640	KMVL 	Morrisville-Stowe State Airport-Morrisville, VT                       	44.534	-72.614	VT 
0101000020E61000000C022B87160152C07F6ABC7493484640	KCDA 	Caledonia County Airport-Lyndonville, VT                              	44.567	-72.017	VT 
0101000020E6100000F4FDD478E90E52C01B2FDD2406714640	KEFK 	Northeast Kingdom International Airport-Newport, VT                   	44.883	-72.233	VT 
0101000020E610000066666666664652C08195438B6C774640	KFSO 	Franklin County State Airport-Highgate, VT                            	44.933	-73.1	VT 
0101000020E61000003C4ED1915CAE5EC08FC2F5285C1F4840	KCVV 	Coupeville-Penn Cove, WA                                              	48.245	-122.7244	WA 
0101000020E61000006ABC749318945EC046B6F3FDD4B84740	KSEA 	Seattle-Tacoma International, WA                                      	47.444	-122.314	WA 
0101000020E6100000068195438B545DC08195438B6CD74740	KSFF 	Felts Field Spokane, WA                                               	47.683	-117.321	WA 
0101000020E61000001904560E2D8A5EC0F853E3A59B144840	KAWO 	Arlington Municipal Airport-Arlington, WA                             	48.161	-122.159	WA 
0101000020E610000054E3A59BC40856C05A643BDF4F3D4640	KGRB 	Green Bay Austin Straubel International Airport, WI                   	44.479	-88.137	WI 
0101000020E6100000273108AC1C6256C06666666666464640	KSTE 	Stevens Point Municipal Airport-Stevens Point, WI                     	44.55	-89.533	WI 
0101000020E6100000D578E926316856C0F4FDD478E9764640	KAUW 	Wausau, WI                                                            	44.929	-89.628	WI 
0101000020E61000001D5A643BDF1757C0D34D621058E94640	KRZN 	Burnett County Airport-Siren, WI                                      	45.823	-92.373	WI 
0101000020E6100000C3F5285C8FBA5AC0FCA9F1D24DD24340	KGEE 	Eagle County - Gypsum, CO                                             	39.643	-106.915	CO 
0101000020E6100000931804560E4D54C0A01A2FDD24A64240	KBLF 	Mercer County Airport-Bluefield, WV                                   	37.298	-81.204	WV 
0101000020E61000001D5A643BDF4754C03108AC1C5AE44240	KBKW 	Beckley Raleigh County Memorial Airport-Beckley, WV                   	37.784	-81.123	WV 
0101000020E61000009A999999991954C0E5D022DBF9EE4240	KLWB 	Greenbrier Valley Airport-Lewisburg, WV                               	37.867	-80.4	WV 
0101000020E6100000EC51B81E85A354C01F85EB51B82E4340	KHTS 	Tri-State Airport-Huntington, WV                                      	38.365	-82.555	WV 
0101000020E6100000F6285C8FC26554C08D976E1283304340	KCRW 	Yeager Airport-Charleston, WV                                         	38.379	-81.59	WV 
0101000020E61000001F85EB51B8F653C052B81E85EB714340	KEKN 	Elkins-Randolph County Airport-Elkins, WV                             	38.89	-79.855	WV 
0101000020E6100000F0A7C64B37C953C0AC1C5A643B7F4340	KW99 	Grant County Airport Petersburg, WV                                   	38.994	-79.144	WV 
0101000020E6100000A8C64B37891154C00000000000804340	KW22 	Upshur County Regional Airport Buckhannon, WV                         	39	-80.274	WV 
0101000020E6100000E17A14AE475154C09A99999999994340	KPKB 	Mid-Ohio Valley Regional Airport-Parkersburg, WV                      	39.2	-81.27	WV 
0101000020E61000002DB29DEFA70E54C0D9CEF753E3A54340	KCKB 	Harrison/Marion Regional Airport-Clarksburg, WV                       	39.296	-80.229	WV 
0101000020E610000014AE47E17A7C53C0C1CAA145B6B34340	KMRB 	Eastern West Virginia Regional-Martinsburg, WV                        	39.404	-77.945	WV 
0101000020E61000006DE7FBA9F1FA53C03333333333D34340	KMGW 	Morgantown Municipal Airport-Morgantown, WV                           	39.65	-79.921	WV 
0101000020E6100000C520B072682954C04A0C022B87164440	KHLG 	Wheeling-Ohio County Airport-Wheeling, WV                             	40.176	-80.647	WV 
0101000020E610000024287E8CB93754C0AEB6627FD91D4340	KSXL 	Summersville Airport-Summersville, WV                                 	38.2332	-80.8707	WV 
\.


--
-- TOC entry 4438 (class 0 OID 29657)
-- Dependencies: 232
-- Data for Name: sua; Type: TABLE DATA; Schema: postgis; Owner: postgres
--

COPY postgis.sua (rep_time, rep_year, rep_num, sched_id, airsp_id, sched_status, airsp_name, start_time, end_time, high_alt, sep_rule, shape_ind, nfdc_id, nfdc_name, dafif_id, dafif_name, airsp_type, low_alt) FROM stdin;
\.


--
-- TOC entry 4435 (class 0 OID 17504)
-- Dependencies: 208
-- Data for Name: taf; Type: TABLE DATA; Schema: postgis; Owner: postgres
--

COPY postgis.taf (issued, current, wind, visby, condx, rep_time, stn_call) FROM stdin;
\.


--
-- TOC entry 4445 (class 0 OID 0)
-- Dependencies: 203
-- Name: nexrad_counter_seq; Type: SEQUENCE SET; Schema: postgis; Owner: postgres
--

SELECT pg_catalog.setval('postgis.nexrad_counter_seq', 1, false);


--
-- TOC entry 4276 (class 2606 OID 17470)
-- Name: airspace_ob_ele airspace_ob_ele_pkey; Type: CONSTRAINT; Schema: postgis; Owner: postgres
--

ALTER TABLE ONLY postgis.airspace_ob_ele
    ADD CONSTRAINT airspace_ob_ele_pkey PRIMARY KEY (airspace_ob_ele_id);


--
-- TOC entry 4288 (class 2606 OID 17502)
-- Name: circles circles_pkey; Type: CONSTRAINT; Schema: postgis; Owner: postgres
--

ALTER TABLE ONLY postgis.circles
    ADD CONSTRAINT circles_pkey PRIMARY KEY (prod_id, rep_num);


--
-- TOC entry 4278 (class 2606 OID 17472)
-- Name: fisb_products fisb_products_pkey; Type: CONSTRAINT; Schema: postgis; Owner: postgres
--

ALTER TABLE ONLY postgis.fisb_products
    ADD CONSTRAINT fisb_products_pkey PRIMARY KEY (fisb_product_id);


--
-- TOC entry 4294 (class 2606 OID 21056)
-- Name: graphics graphics_pkey; Type: CONSTRAINT; Schema: postgis; Owner: postgres
--

ALTER TABLE ONLY postgis.graphics
    ADD CONSTRAINT graphics_pkey PRIMARY KEY (rep_num, prod_id, alt);


--
-- TOC entry 4280 (class 2606 OID 17476)
-- Name: metar metar_pkey; Type: CONSTRAINT; Schema: postgis; Owner: postgres
--

ALTER TABLE ONLY postgis.metar
    ADD CONSTRAINT metar_pkey PRIMARY KEY (stn_call, ob_date);


--
-- TOC entry 4298 (class 2606 OID 39726)
-- Name: nexrad nexrad84_pkey; Type: CONSTRAINT; Schema: postgis; Owner: postgres
--

ALTER TABLE ONLY postgis.nexrad
    ADD CONSTRAINT nexrad84_pkey PRIMARY KEY (prod_id, cc, block_num, maptime, altitude, coords);


--
-- TOC entry 4282 (class 2606 OID 17480)
-- Name: overlay_geo_opt overlay_geo_opt_pkey; Type: CONSTRAINT; Schema: postgis; Owner: postgres
--

ALTER TABLE ONLY postgis.overlay_geo_opt
    ADD CONSTRAINT overlay_geo_opt_pkey PRIMARY KEY (overlay_geo_opt_id);


--
-- TOC entry 4284 (class 2606 OID 17482)
-- Name: pirep pirep_pkey; Type: CONSTRAINT; Schema: postgis; Owner: postgres
--

ALTER TABLE ONLY postgis.pirep
    ADD CONSTRAINT pirep_pkey PRIMARY KEY (rep_time, stn_call);


--
-- TOC entry 4292 (class 2606 OID 21045)
-- Name: sigairmet sigairmet_pkey; Type: CONSTRAINT; Schema: postgis; Owner: postgres
--

ALTER TABLE ONLY postgis.sigairmet
    ADD CONSTRAINT sigairmet_pkey PRIMARY KEY (prod_id, rep_num, stn_call);


--
-- TOC entry 4286 (class 2606 OID 21075)
-- Name: stations stations_pkey; Type: CONSTRAINT; Schema: postgis; Owner: postgres
--

ALTER TABLE ONLY postgis.stations
    ADD CONSTRAINT stations_pkey PRIMARY KEY (stn_call);


--
-- TOC entry 4296 (class 2606 OID 29664)
-- Name: sua sua_pkey; Type: CONSTRAINT; Schema: postgis; Owner: postgres
--

ALTER TABLE ONLY postgis.sua
    ADD CONSTRAINT sua_pkey PRIMARY KEY (rep_num, sched_status);


--
-- TOC entry 4290 (class 2606 OID 17511)
-- Name: taf taf_pkey; Type: CONSTRAINT; Schema: postgis; Owner: postgres
--

ALTER TABLE ONLY postgis.taf
    ADD CONSTRAINT taf_pkey PRIMARY KEY (stn_call, rep_time);


-- Completed on 2021-04-20 14:01:19 EDT

--
-- PostgreSQL database dump complete
--

