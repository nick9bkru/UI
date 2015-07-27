--
-- PostgreSQL database dump
--

SET statement_timeout = 0;
SET lock_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;

SET search_path = public, pg_catalog;

SET default_tablespace = '';

SET default_with_oids = true;

--
-- Name: sost_sa_rpu; Type: TABLE; Schema: public; Owner: root; Tablespace: 
--

CREATE TABLE sost_sa_rpu (
    num smallint NOT NULL,
    from_a integer,
    to_a integer,
    nport smallint,
    port smallint,
    factory_number character(20),
    keyzone smallint,
    ki smallint,
    vs smallint,
    sendbuf smallint,
    dt_key timestamp without time zone
);


ALTER TABLE public.sost_sa_rpu OWNER TO root;

--
-- Data for Name: sost_sa_rpu; Type: TABLE DATA; Schema: public; Owner: root
--

COPY sost_sa_rpu (num, from_a, to_a, nport, port, factory_number, keyzone, ki, vs, sendbuf, dt_key) FROM stdin;
13	0	145	2	5	001096              	0	1	0	145	2012-10-11 16:59:19.588579
14	0	145	2	6	001094              	5	0	0	145	2012-10-11 16:59:20.072534
15	0	145	2	7	001098              	5	0	0	145	2012-10-11 16:59:20.504544
16	0	145	2	8	001097              	5	0	0	145	2013-08-06 16:18:03.256825
17	0	145	2	9	001112              	5	0	1	145	2012-10-11 17:04:48.975096
18	0	145	2	10	123354              	5	0	0	145	2013-08-06 16:18:50.780071
19	0	145	2	11	001111              	5	0	0	145	2012-10-11 16:49:02.258946
20	0	145	2	12	001105              	5	0	0	145	2012-10-11 16:35:56.436707
21	0	145	3	1	001107              	26	0	0	145	2012-11-27 21:38:26.285458
22	0	145	3	2	001071              	5	0	0	145	2012-10-11 16:53:13.487512
23	0	145	3	3	001088              	5	0	0	145	2012-10-11 16:53:13.925394
24	0	145	3	4	001089              	5	0	0	145	2012-10-11 17:03:15.383876
25	0	145	3	5	001101              	5	0	0	145	2012-10-31 15:22:58.819301
26	0	145	3	6	fbdfgdfg            	5	0	0	145	2012-10-11 16:56:12.119699
27	0	145	3	7	7643                	5	0	0	145	2012-10-11 16:57:21.199717
28	0	145	3	8	dfdsfsdfdf          	5	0	0	145	2012-10-11 16:57:56.035323
29	0	145	3	9	11111111            	5	0	0	145	2012-10-11 16:59:27.397408
30	0	145	3	10	hggfjfhj            	8	0	0	145	2012-10-11 16:59:27.830376
31	0	145	3	11	4444                	5	0	0	145	2012-10-11 16:59:18.077643
32	0	145	3	12	                    	5	0	0	145	2012-10-11 16:59:23.605497
33	0	161	4	1	45443333            	5	0	0	161	2012-10-11 17:01:05.120931
34	0	161	4	2	                    	5	0	0	161	2012-10-11 16:34:50.719215
35	0	161	4	3	                    	5	0	0	161	2012-10-11 16:34:51.181197
36	0	161	4	4	                    	5	0	0	161	2012-10-11 16:34:47.801388
37	0	145	4	5	                    	5	0	0	145	2012-10-11 16:53:07.271689
38	0	145	4	6	                    	5	0	0	145	2012-10-11 16:53:11.058539
39	0	145	4	7	                    	5	0	0	145	2013-09-21 03:29:18.74634
40	0	161	4	8	                    	5	0	0	161	2012-10-11 16:53:11.935506
42	37648	7	2	0	                    	\N	0	0	2	\N
43	37648	7	3	0	                    	\N	0	0	2	\N
44	37648	7	4	0	                    	\N	0	0	2	\N
45	37648	7	5	0	                    	0	0	0	2	\N
46	37648	7	6	0	                    	\N	0	0	2	\N
1	0	145	1	1	001086              	1	0	0	145	2012-10-11 16:56:50.744188
2	0	145	1	2	000334              	5	0	0	145	2012-11-22 15:45:45.286604
3	0	145	1	3	001089              	21	1	0	145	2012-10-11 16:56:29.814658
4	0	145	1	4	001014              	\N	0	1	145	2012-10-11 16:48:47.451528
5	0	145	1	5	001115              	\N	1	0	145	2012-10-12 11:05:53.672671
47	37648	7	7	0	                    	\N	0	0	2	\N
48	37648	7	8	0	                    	\N	0	0	2	\N
6	0	145	1	6	001081              	\N	0	0	145	2012-10-11 16:53:12.164487
7	0	145	1	7	001103              	\N	0	0	145	2012-10-11 16:53:12.858631
8	0	145	1	8	001093              	\N	0	0	145	\N
9	0	145	2	1	001109              	\N	0	0	145	2012-10-11 16:56:10.800856
41	37648	7	1	0	                    	\N	0	0	2	\N
10	0	145	2	2	001110              	\N	0	0	145	2012-10-11 16:56:21.332417
11	0	145	2	3	001114              	\N	0	0	145	2012-10-11 16:56:16.66559
12	0	145	2	4	001106              	\N	0	0	145	2012-10-11 16:56:07.038998
\.


--
-- Name: sost_sa_rpu_pkey; Type: CONSTRAINT; Schema: public; Owner: root; Tablespace: 
--

ALTER TABLE ONLY sost_sa_rpu
    ADD CONSTRAINT sost_sa_rpu_pkey PRIMARY KEY (num);


--
-- Name: change_sost_sa_rpu; Type: TRIGGER; Schema: public; Owner: root
--

CREATE TRIGGER change_sost_sa_rpu AFTER INSERT OR DELETE OR UPDATE ON sost_sa_rpu FOR EACH ROW EXECUTE PROCEDURE sost_sa_rpu_trigger();


--
-- PostgreSQL database dump complete
--

