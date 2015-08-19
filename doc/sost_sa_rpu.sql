--
-- PostgreSQL database dump
--

SET client_encoding = 'UTF8';
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

INSERT INTO sost_sa_rpu VALUES (21, 0, 161, 3, 1, '001107              ', 26, 0, 0, 161, '2012-11-27 21:38:26.285458');
INSERT INTO sost_sa_rpu VALUES (1, 0, 161, 1, 1, '001009              ', 1, 0, 0, 161, '2012-10-11 16:56:50.744188');
INSERT INTO sost_sa_rpu VALUES (9, 0, 161, 2, 1, '001109              ', NULL, 0, 0, 161, '2012-10-11 16:56:10.800856');
INSERT INTO sost_sa_rpu VALUES (2, 0, 161, 1, 2, '000334              ', 5, 0, 0, 161, '2012-11-22 15:45:45.286604');
INSERT INTO sost_sa_rpu VALUES (22, 0, 161, 3, 2, '001071              ', 5, 0, 0, 161, '2012-10-11 16:53:13.487512');
INSERT INTO sost_sa_rpu VALUES (10, 0, 161, 2, 2, '001110              ', NULL, 0, 0, 161, '2012-10-11 16:56:21.332417');
INSERT INTO sost_sa_rpu VALUES (35, 0, 161, 4, 3, '                    ', 5, 0, 0, 161, '2012-10-11 16:34:51.181197');
INSERT INTO sost_sa_rpu VALUES (3, 0, 161, 1, 3, '001038              ', 21, 1, 0, 161, '2012-10-11 16:56:29.814658');
INSERT INTO sost_sa_rpu VALUES (23, 0, 161, 3, 3, '001088              ', 5, 0, 0, 161, '2012-10-11 16:53:13.925394');
INSERT INTO sost_sa_rpu VALUES (36, 0, 161, 4, 4, '                    ', 5, 0, 0, 161, '2012-10-11 16:34:47.801388');
INSERT INTO sost_sa_rpu VALUES (4, 0, 161, 1, 4, '001014              ', NULL, 0, 1, 161, '2012-10-11 16:48:47.451528');
INSERT INTO sost_sa_rpu VALUES (11, 0, 145, 2, 3, '001114              ', NULL, 0, 0, 145, '2012-10-11 16:56:16.66559');
INSERT INTO sost_sa_rpu VALUES (37, 0, 161, 4, 5, '                    ', 5, 0, 0, 161, '2012-10-11 16:53:07.271689');
INSERT INTO sost_sa_rpu VALUES (5, 0, 161, 1, 5, '001115              ', NULL, 1, 0, 161, '2012-10-12 11:05:53.672671');
INSERT INTO sost_sa_rpu VALUES (24, 0, 161, 3, 4, '001089              ', 5, 0, 0, 161, '2012-10-11 17:03:15.383876');
INSERT INTO sost_sa_rpu VALUES (12, 0, 161, 2, 4, '001106              ', NULL, 0, 0, 161, '2012-10-11 16:56:07.038998');
INSERT INTO sost_sa_rpu VALUES (38, 0, 161, 4, 6, '                    ', 5, 0, 0, 161, '2012-10-11 16:53:11.058539');
INSERT INTO sost_sa_rpu VALUES (6, 0, 161, 1, 6, '001081              ', NULL, 0, 0, 161, '2012-10-11 16:53:12.164487');
INSERT INTO sost_sa_rpu VALUES (25, 0, 161, 3, 5, '001101              ', 5, 0, 0, 161, '2012-10-31 15:22:58.819301');
INSERT INTO sost_sa_rpu VALUES (39, 0, 161, 4, 7, '                    ', 5, 0, 0, 161, '2012-10-11 16:53:11.493572');
INSERT INTO sost_sa_rpu VALUES (7, 0, 161, 1, 7, '001103              ', NULL, 0, 0, 161, '2012-10-11 16:53:12.858631');
INSERT INTO sost_sa_rpu VALUES (13, 0, 145, 2, 5, '001096              ', 0, 1, 0, 145, '2012-10-11 16:59:19.588579');
INSERT INTO sost_sa_rpu VALUES (40, 0, 161, 4, 8, '                    ', 5, 0, 0, 161, '2012-10-11 16:53:11.935506');
INSERT INTO sost_sa_rpu VALUES (26, 0, 161, 3, 6, 'fbdfgdfg            ', 5, 0, 0, 161, '2012-10-11 16:56:12.119699');
INSERT INTO sost_sa_rpu VALUES (8, 0, 161, 1, 8, '001093              ', NULL, 0, 0, 161, NULL);
INSERT INTO sost_sa_rpu VALUES (14, 0, 161, 2, 6, '001094              ', 5, 0, 0, 161, '2012-10-11 16:59:20.072534');
INSERT INTO sost_sa_rpu VALUES (27, 0, 161, 3, 7, '7643                ', 5, 0, 0, 161, '2012-10-11 16:57:21.199717');
INSERT INTO sost_sa_rpu VALUES (15, 0, 161, 2, 7, '001098              ', 5, 0, 0, 161, '2012-10-11 16:59:20.504544');
INSERT INTO sost_sa_rpu VALUES (28, 0, 161, 3, 8, 'dfdsfsdfdf          ', 5, 0, 0, 161, '2012-10-11 16:57:56.035323');
INSERT INTO sost_sa_rpu VALUES (16, 0, 161, 2, 8, '001097              ', 5, 0, 0, 161, '2012-10-11 16:59:31.020162');
INSERT INTO sost_sa_rpu VALUES (29, 0, 145, 3, 9, '11111111            ', 5, 0, 0, 145, '2012-10-11 16:59:27.397408');
INSERT INTO sost_sa_rpu VALUES (17, 0, 161, 2, 9, '001112              ', 5, 0, 1, 161, '2012-10-11 17:04:48.975096');
INSERT INTO sost_sa_rpu VALUES (30, 0, 161, 3, 10, 'hggfjfhj            ', 8, 0, 0, 161, '2012-10-11 16:59:27.830376');
INSERT INTO sost_sa_rpu VALUES (18, 0, 161, 2, 10, '123354              ', 5, 0, 0, 161, '2012-10-11 16:36:22.336828');
INSERT INTO sost_sa_rpu VALUES (31, 0, 161, 3, 11, '4444                ', 5, 0, 0, 161, '2012-10-11 16:59:18.077643');
INSERT INTO sost_sa_rpu VALUES (19, 0, 161, 2, 11, '001111              ', 5, 0, 0, 161, '2012-10-11 16:49:02.258946');
INSERT INTO sost_sa_rpu VALUES (32, 0, 161, 3, 12, '                    ', 5, 0, 0, 161, '2012-10-11 16:59:23.605497');
INSERT INTO sost_sa_rpu VALUES (20, 0, 145, 2, 12, '001105              ', 5, 0, 0, 145, '2012-10-11 16:35:56.436707');
INSERT INTO sost_sa_rpu VALUES (41, 4352, 2, 36, -1, '                    ', NULL, 0, 0, 2, NULL);
INSERT INTO sost_sa_rpu VALUES (48, 768, 2, 43, -1, '                    ', NULL, 0, 0, 2, NULL);
INSERT INTO sost_sa_rpu VALUES (42, 768, 2, 37, -1, '                    ', NULL, 0, 0, 2, NULL);
INSERT INTO sost_sa_rpu VALUES (46, 768, 2, 41, -1, '                    ', NULL, 0, 0, 2, NULL);
INSERT INTO sost_sa_rpu VALUES (43, 768, 2, 38, -1, '                    ', NULL, 0, 0, 2, NULL);
INSERT INTO sost_sa_rpu VALUES (47, 768, 2, 42, -1, '                    ', NULL, 0, 0, 2, NULL);
INSERT INTO sost_sa_rpu VALUES (44, 768, 2, 39, -1, '                    ', NULL, 0, 0, 2, NULL);
INSERT INTO sost_sa_rpu VALUES (45, 768, 2, 40, -1, '                    ', 0, 0, 0, 2, NULL);
INSERT INTO sost_sa_rpu VALUES (33, 0, 145, 4, 1, '45443333            ', 5, 0, 0, 145, '2012-10-11 17:01:05.120931');
INSERT INTO sost_sa_rpu VALUES (34, 0, 161, 4, 2, '                    ', 5, 0, 0, 161, '2012-10-11 16:34:50.719215');


--
-- Name: sost_sa_rpu_pkey; Type: CONSTRAINT; Schema: public; Owner: root; Tablespace: 
--

ALTER TABLE ONLY sost_sa_rpu
    ADD CONSTRAINT sost_sa_rpu_pkey PRIMARY KEY (num);


--
-- Name: change_sost_sa_rpu; Type: TRIGGER; Schema: public; Owner: root
--

CREATE TRIGGER change_sost_sa_rpu
    AFTER INSERT OR DELETE OR UPDATE ON sost_sa_rpu
    FOR EACH ROW
    EXECUTE PROCEDURE sost_sa_rpu_trigger();


--
-- PostgreSQL database dump complete
--

