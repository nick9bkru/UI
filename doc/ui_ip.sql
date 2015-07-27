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

SET default_with_oids = false;

--
-- Name: ui_ip; Type: TABLE; Schema: public; Owner: nick; Tablespace: 
--

CREATE TABLE ui_ip (
    index integer,
    name text,
    ip inet,
    type integer,
    sost boolean DEFAULT false
);


ALTER TABLE public.ui_ip OWNER TO nick;

--
-- Data for Name: ui_ip; Type: TABLE DATA; Schema: public; Owner: nick
--

COPY ui_ip (index, name, ip, type, sost) FROM stdin;
5	УИ РМИШ	192.168.1.115	1	f
6	УИ РМИШ	192.168.1.116	1	f
7	УИ РМИШ	192.168.1.117	1	f
8	УИ РМИШ	192.168.1.118	1	f
9	УИ РМИШ	192.168.1.119	1	f
10	УИ РМИШ	192.168.1.120	1	f
11	УИ РМИШ	192.168.1.121	1	f
1	УИ РМИШ	192.168.1.111	1	t
0	УИ РМИШ	192.168.1.110	1	t
2	УИ РМИШ	192.168.1.112	1	t
3	УИ РМИШ	192.168.1.113	1	t
4	УИ РМИШ	192.168.1.114	1	t
\.


--
-- Name: index_unique; Type: CONSTRAINT; Schema: public; Owner: nick; Tablespace: 
--

ALTER TABLE ONLY ui_ip
    ADD CONSTRAINT index_unique UNIQUE (index);


--
-- PostgreSQL database dump complete
--

