--
-- PostgreSQL database dump
--

SET statement_timeout = 0;
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
4	0	145	1	4	001014              	\N	0	1	145	2012-10-11 16:48:47.451528
12	0	145	2	4	001106              	\N	0	0	145	2012-10-11 16:56:07.038998
36	0	161	4	4	                    	5	0	0	161	2012-10-11 16:34:47.801388
24	0	145	3	4	001089              	5	0	0	145	2012-10-11 17:03:15.383876
5	0	145	1	5	001115              	\N	1	0	145	2012-10-12 11:05:53.672671
13	0	145	2	5	001096              	0	1	0	145	2012-10-11 16:59:19.588579
37	0	145	4	5	                    	5	0	0	145	2012-10-11 16:53:07.271689
14	0	145	2	6	001094              	5	0	0	145	2012-10-11 16:59:20.072534
6	0	145	1	6	001081              	\N	0	0	145	2012-10-11 16:53:12.164487
25	0	145	3	5	001101              	5	0	0	145	2012-10-31 15:22:58.819301
38	0	145	4	6	                    	5	0	0	145	2012-10-11 16:53:11.058539
15	0	145	2	7	001098              	5	0	0	145	2012-10-11 16:59:20.504544
7	0	145	1	7	001103              	\N	0	0	145	2012-10-11 16:53:12.858631
39	0	145	4	7	                    	5	0	0	145	2013-09-21 03:29:18.74634
26	0	145	3	6	fbdfgdfg            	5	0	0	145	2012-10-11 16:56:12.119699
8	0	145	1	8	001093              	\N	0	0	145	\N
40	0	161	4	8	                    	5	0	0	161	2012-10-11 16:53:11.935506
16	0	145	2	8	001097              	5	0	0	145	2013-08-06 16:18:03.256825
27	0	145	3	7	7643                	5	0	0	145	2012-10-11 16:57:21.199717
17	0	145	2	9	001112              	5	0	1	145	2012-10-11 17:04:48.975096
28	0	145	3	8	dfdsfsdfdf          	5	0	0	145	2012-10-11 16:57:56.035323
18	0	145	2	10	123354              	5	0	0	145	2013-08-06 16:18:50.780071
19	0	145	2	11	001111              	5	0	0	145	2012-10-11 16:49:02.258946
29	0	145	3	9	11111111            	5	0	0	145	2012-10-11 16:59:27.397408
20	0	145	2	12	001105              	5	0	0	145	2012-10-11 16:35:56.436707
30	0	145	3	10	hggfjfhj            	8	0	0	145	2012-10-11 16:59:27.830376
31	0	145	3	11	4444                	5	0	0	145	2012-10-11 16:59:18.077643
32	0	145	3	12	                    	5	0	0	145	2012-10-11 16:59:23.605497
41	0	0	36	0	                    	\N	0	0	2	\N
42	0	0	37	0	                    	\N	0	0	2	\N
45	0	0	40	0	                    	0	0	0	2	\N
33	0	161	4	1	45443333            	5	0	0	161	2012-10-11 17:01:05.120931
21	0	145	3	1	001107              	26	0	0	145	2012-11-27 21:38:26.285458
9	0	145	2	1	001109              	\N	0	0	145	2012-10-11 16:56:10.800856
1	0	145	1	1	001086              	1	0	0	145	2012-10-11 16:56:50.744188
34	0	161	4	2	                    	5	0	0	161	2012-10-11 16:34:50.719215
46	0	0	41	0	                    	\N	0	0	2	\N
2	0	145	1	2	000334              	5	0	0	145	2012-11-22 15:45:45.286604
10	0	145	2	2	001110              	\N	0	0	145	2012-10-11 16:56:21.332417
22	0	145	3	2	001071              	5	0	0	145	2012-10-11 16:53:13.487512
3	0	145	1	3	001089              	21	1	0	145	2012-10-11 16:56:29.814658
35	0	161	4	3	                    	5	0	0	161	2012-10-11 16:34:51.181197
47	0	0	42	0	                    	\N	0	0	2	\N
43	0	0	38	0	                    	\N	0	0	2	\N
11	0	145	2	3	001114              	\N	0	0	145	2012-10-11 16:56:16.66559
48	0	0	43	0	                    	\N	0	0	2	\N
44	0	0	39	0	                    	\N	0	0	2	\N
23	0	145	3	3	001088              	5	0	0	145	2012-10-11 16:53:13.925394
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

