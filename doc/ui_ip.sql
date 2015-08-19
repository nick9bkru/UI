--
-- PostgreSQL database dump
--

SET client_encoding = 'UTF8';
SET check_function_bodies = false;
SET client_min_messages = warning;

SET search_path = public, pg_catalog;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: ui_ip; Type: TABLE; Schema: public; Owner: root; Tablespace: 
--

CREATE TABLE ui_ip (
    "index" integer,
    name text,
    ip inet,
    "type" integer,
    sost integer DEFAULT 0,
    rmdi_ind integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.ui_ip OWNER TO root;

--
-- Data for Name: ui_ip; Type: TABLE DATA; Schema: public; Owner: root
--

INSERT INTO ui_ip VALUES (24, 'УИ УПС', '192.168.1.134', 1, 1, 22);
INSERT INTO ui_ip VALUES (17, 'ГШ', '192.168.1.127', 0, 1, 17);
INSERT INTO ui_ip VALUES (29, 'УИ УПС', '192.168.1.139', 1, 1, 27);
INSERT INTO ui_ip VALUES (14, 'ГШ', '192.168.1.124', 0, 1, 14);
INSERT INTO ui_ip VALUES (32, 'УИ УПС', '192.168.1.146', 1, 1, 30);
INSERT INTO ui_ip VALUES (5, 'УИ РМИШ', '192.168.1.115', 0, 1, 5);
INSERT INTO ui_ip VALUES (22, 'ГШ', '192.168.1.132', 0, 1, -1);
INSERT INTO ui_ip VALUES (12, 'ГШ', '192.168.1.122', 0, 1, 12);
INSERT INTO ui_ip VALUES (15, 'ГШ', '192.168.1.125', 0, 1, 15);
INSERT INTO ui_ip VALUES (7, 'УИ РМИШ', '192.168.1.117', 0, 1, 7);
INSERT INTO ui_ip VALUES (9, 'УИ РМИШ', '192.168.1.119', 0, 1, 9);
INSERT INTO ui_ip VALUES (23, 'ГШ', '192.168.1.133', 0, 1, -1);
INSERT INTO ui_ip VALUES (18, 'ГШ', '192.168.1.128', 0, 1, 18);
INSERT INTO ui_ip VALUES (13, 'ГШ', '192.168.1.123', 0, 1, 13);
INSERT INTO ui_ip VALUES (20, 'ГШ', '192.168.1.130', 0, 1, 20);
INSERT INTO ui_ip VALUES (33, 'УИ УПС', '192.168.1.147', 1, 1, 31);
INSERT INTO ui_ip VALUES (2, 'УИ РМИШ', '192.168.1.112', 0, 1, 2);
INSERT INTO ui_ip VALUES (25, 'УИ УПС', '192.168.1.135', 1, 1, 23);
INSERT INTO ui_ip VALUES (21, 'ГШ', '192.168.1.131', 0, 1, 21);
INSERT INTO ui_ip VALUES (35, 'УИ УПС', '192.168.1.149', 1, 1, 33);
INSERT INTO ui_ip VALUES (0, 'УИ РМИШ', '192.168.1.110', 0, 1, 0);
INSERT INTO ui_ip VALUES (16, 'ГШ', '192.168.1.126', 0, 1, 16);
INSERT INTO ui_ip VALUES (19, 'ГШ', '192.168.1.129', 0, 1, 19);
INSERT INTO ui_ip VALUES (27, 'УИ УПС', '192.168.1.137', 1, 1, 25);
INSERT INTO ui_ip VALUES (31, 'УИ УПС', '192.168.1.141', 1, 1, 29);
INSERT INTO ui_ip VALUES (3, 'УИ РМИШ', '192.168.1.113', 0, 1, 3);
INSERT INTO ui_ip VALUES (8, 'УИ РМИШ', '192.168.1.118', 0, 1, 8);
INSERT INTO ui_ip VALUES (6, 'УИ РМИШ', '192.168.1.116', 0, 1, 6);
INSERT INTO ui_ip VALUES (1, 'УИ РМИШ', '192.168.1.111', 0, 1, 1);
INSERT INTO ui_ip VALUES (28, 'УИ УПС', '192.168.1.138', 1, 1, 26);
INSERT INTO ui_ip VALUES (4, 'УИ РМИШ', '192.168.1.114', 0, 1, 4);
INSERT INTO ui_ip VALUES (34, 'УИ УПС', '192.168.1.148', 1, 1, 32);
INSERT INTO ui_ip VALUES (26, 'УИ УПС', '192.168.1.136', 1, 1, 24);
INSERT INTO ui_ip VALUES (30, 'УИ УПС', '192.168.1.140', 1, 1, 28);
INSERT INTO ui_ip VALUES (43, 'УИ АРПУ', '192.168.1.165', 2, 1, 41);
INSERT INTO ui_ip VALUES (36, 'УИ АРПУ', '192.168.1.158', 2, 1, 34);
INSERT INTO ui_ip VALUES (42, 'УИ АРПУ', '192.168.1.164', 2, 1, 40);
INSERT INTO ui_ip VALUES (41, 'УИ АРПУ', '192.168.1.163', 2, 1, 39);
INSERT INTO ui_ip VALUES (38, 'УИ АРПУ', '192.168.1.160', 2, 1, 36);
INSERT INTO ui_ip VALUES (37, 'УИ АРПУ', '192.168.1.159', 2, 1, 35);
INSERT INTO ui_ip VALUES (39, 'УИ АРПУ', '192.168.1.161', 2, 1, 37);
INSERT INTO ui_ip VALUES (40, 'УИ АРПУ', '192.168.1.162', 2, 1, 38);


--
-- Name: index_unique; Type: CONSTRAINT; Schema: public; Owner: root; Tablespace: 
--

ALTER TABLE ONLY ui_ip
    ADD CONSTRAINT index_unique UNIQUE ("index");


--
-- Name: ui_ip_index; Type: INDEX; Schema: public; Owner: root; Tablespace: 
--

CREATE INDEX ui_ip_index ON ui_ip USING btree ("index");


--
-- Name: change_uso; Type: TRIGGER; Schema: public; Owner: root
--

CREATE TRIGGER change_uso
    AFTER INSERT OR DELETE OR UPDATE ON ui_ip
    FOR EACH ROW
    EXECUTE PROCEDURE uso_trigger();


--
-- PostgreSQL database dump complete
--

