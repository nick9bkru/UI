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
-- Name: nsd; Type: TABLE; Schema: public; Owner: root; Tablespace: 
--

CREATE TABLE nsd (
    name text,
    but_id integer,
    state integer DEFAULT 0,
    in_signal integer,
    out_signal integer,
    line_id smallint,
    ko integer,
    "index" integer,
    ui integer,
    ui_contact integer
);


ALTER TABLE public.nsd OWNER TO root;

--
-- Data for Name: nsd; Type: TABLE DATA; Schema: public; Owner: root
--

INSERT INTO nsd VALUES ('ФП1 КО', 43, 17, 1, 2, 5, 60, 82, 12, 2);
INSERT INTO nsd VALUES ('ЗШКО в ', 32, 6, 2, 2, 31, 33, 65, 5, 1);
INSERT INTO nsd VALUES ('ФП2 КО', 44, 17, 1, 2, 6, 60, 83, 12, 3);
INSERT INTO nsd VALUES ('БП1 ГШ', 41, 17, 1, 2, 3, 48, 80, 22, 0);
INSERT INTO nsd VALUES ('РМДИ2 з', 19, 4, 1, 2, 76, 20, 39, 20, 3);
INSERT INTO nsd VALUES ('ЗШКО н', 32, 6, 1, 2, 32, 33, 64, 5, 0);
INSERT INTO nsd VALUES ('БП2 ГШ', 42, 17, 1, 1, 4, 48, 81, 22, 1);
INSERT INTO nsd VALUES ('К05М 37-40 н', 30, 6, 1, 2, 28, 31, 60, 4, 0);
INSERT INTO nsd VALUES ('РМДИ1 п', 18, 4, 1, 2, 73, 19, 36, 20, 0);
INSERT INTO nsd VALUES ('KAPC CA 1 в', 31, 6, 2, 2, 30, 32, 63, 5, 3);
INSERT INTO nsd VALUES ('ПА616 1', 39, 17, 1, 2, 1, 48, 78, 13, 2);
INSERT INTO nsd VALUES ('KAPC CA 2 н', 31, 6, 1, 2, 29, 32, 62, 5, 2);
INSERT INTO nsd VALUES ('РМДИ1 з', 18, 4, 1, 2, 74, 19, 37, 20, 1);
INSERT INTO nsd VALUES ('ПА616 2', 40, 17, 1, 2, 2, 48, 79, 13, 3);
INSERT INTO nsd VALUES ('РМДИ2 п', 19, 0, 1, 2, 75, 20, 38, 20, 2);
INSERT INTO nsd VALUES ('ЯШ136 Эл. пит. п', 13, 4, 2, 2, 45, 14, 26, 12, 0);
INSERT INTO nsd VALUES ('ЯШ136 Эл. пит. з', 13, 4, 1, 2, 46, 14, 27, 12, 1);
INSERT INTO nsd VALUES ('К05М 5-8 в', 22, 6, 1, 2, 12, 23, 45, 0, 1);
INSERT INTO nsd VALUES ('К05М 5-8 н', 22, 6, 2, 2, 11, 23, 44, 0, 0);
INSERT INTO nsd VALUES ('ФП2 ГШ', 46, 17, 1, 2, 8, 60, 85, 21, 1);
INSERT INTO nsd VALUES ('П616 з', 0, 5, 2, 2, 48, 1, 1, 13, 0);
INSERT INTO nsd VALUES ('К05М 9-12 н', 23, 6, 2, 2, 13, 24, 46, 1, 2);
INSERT INTO nsd VALUES ('К05М 9-12 в', 23, 6, 1, 2, 14, 24, 47, 1, 3);
INSERT INTO nsd VALUES ('П616 п', 0, 5, 2, 2, 47, 1, 0, 13, 0);
INSERT INTO nsd VALUES ('К05М 29-32 н', 28, 6, 1, 2, 24, 29, 56, 3, 0);
INSERT INTO nsd VALUES ('К05М 29-32 в', 28, 6, 1, 2, 23, 29, 57, 3, 1);
INSERT INTO nsd VALUES ('ЯШ096-1 ГШ п', 7, 6, 1, 2, 81, 8, 14, 23, 2);
INSERT INTO nsd VALUES ('КО ЛВС н', 34, 4, 1, 2, 36, 35, 68, 6, 0);
INSERT INTO nsd VALUES ('ОКС МТ п', 36, 4, 1, 2, 37, 37, 72, 7, 0);
INSERT INTO nsd VALUES ('ОКС МТ з', 36, 4, 1, 2, 38, 37, 73, 7, 1);
INSERT INTO nsd VALUES ('К05М 25-28 в', 27, 6, 1, 2, 21, 28, 55, 3, 3);
INSERT INTO nsd VALUES ('К05М 25-28 н', 27, 6, 1, 2, 22, 28, 54, 3, 2);
INSERT INTO nsd VALUES ('К05М 17-20 н', 25, 6, 2, 2, 17, 26, 50, 2, 2);
INSERT INTO nsd VALUES ('К05М 17-20 в', 25, 6, 1, 2, 18, 26, 51, 2, 3);
INSERT INTO nsd VALUES ('ЦУУ1 п', 4, 6, 1, 2, 55, 5, 8, 15, 0);
INSERT INTO nsd VALUES ('ЦУУ1 з', 4, 6, 1, 2, 56, 5, 9, 15, 1);
INSERT INTO nsd VALUES ('СА508 ОКС БИ п', 2, 4, 1, 2, 51, 3, 4, 14, 0);
INSERT INTO nsd VALUES ('СА508 ОКС БИ з', 2, 4, 1, 2, 52, 3, 5, 14, 1);
INSERT INTO nsd VALUES ('К05М 33-36 н', 29, 6, 1, 2, 26, 30, 58, 4, 2);
INSERT INTO nsd VALUES ('ЦКО п', 3, 4, 1, 2, 53, 4, 6, 14, 2);
INSERT INTO nsd VALUES ('ЦКО з', 3, 4, 1, 2, 54, 4, 7, 14, 3);
INSERT INTO nsd VALUES ('ЦУУ2 п', 5, 6, 1, 2, 57, 6, 10, 15, 2);
INSERT INTO nsd VALUES ('ЦУУ2 з', 5, 6, 1, 2, 58, 6, 11, 15, 3);
INSERT INTO nsd VALUES ('КАРС АС п', 20, 4, 1, 2, 59, 21, 40, 16, 0);
INSERT INTO nsd VALUES ('КАРС АС з', 20, 4, 1, 2, 60, 21, 41, 16, 1);
INSERT INTO nsd VALUES ('кросс НС,Ц п', 6, 6, 1, 2, 79, 7, 12, 23, 0);
INSERT INTO nsd VALUES ('кросс НС,Ц з', 6, 6, 1, 2, 80, 7, 13, 23, 1);
INSERT INTO nsd VALUES ('ЯШ096-1 ГШ з', 7, 6, 1, 2, 82, 8, 15, 23, 3);
INSERT INTO nsd VALUES ('К05М 21-24 н', 26, 6, 2, 2, 20, 27, 52, 2, 0);
INSERT INTO nsd VALUES ('К05М 21-24 в', 26, 6, 1, 2, 19, 27, 53, 2, 1);
INSERT INTO nsd VALUES ('кросс С п', 12, 4, 1, 2, 77, 13, 24, 21, 0);
INSERT INTO nsd VALUES ('ФП1 ГШ', 45, 17, 1, 2, 7, 60, 84, 21, 0);
INSERT INTO nsd VALUES ('К05М 13-16 н', 24, 6, 2, 2, 15, 25, 48, 1, 0);
INSERT INTO nsd VALUES ('ОКС ТОФМ п ', 10, 4, 1, 2, 65, 11, 20, 18, 0);
INSERT INTO nsd VALUES ('ОКС ТОФМ з', 10, 4, 1, 2, 66, 11, 21, 18, 1);
INSERT INTO nsd VALUES ('УПС1 п', 11, 4, 1, 2, 67, 12, 22, 18, 2);
INSERT INTO nsd VALUES ('УПС1 з', 11, 4, 1, 2, 68, 12, 23, 18, 3);
INSERT INTO nsd VALUES ('РМО1 п', 14, 4, 1, 2, 69, 15, 28, 19, 0);
INSERT INTO nsd VALUES ('РМО1 з', 14, 4, 1, 2, 70, 15, 29, 19, 1);
INSERT INTO nsd VALUES ('РМО2 п', 15, 4, 1, 2, 71, 16, 30, 19, 2);
INSERT INTO nsd VALUES ('РМО2 з', 15, 4, 1, 2, 72, 16, 31, 19, 3);
INSERT INTO nsd VALUES ('УПС3 п', 9, 4, 1, 2, 63, 10, 18, 17, 2);
INSERT INTO nsd VALUES ('УПС3 з', 9, 4, 1, 2, 64, 10, 19, 17, 3);
INSERT INTO nsd VALUES ('УПС2 п', 8, 4, 1, 2, 61, 9, 16, 17, 0);
INSERT INTO nsd VALUES ('УПС2 з', 8, 4, 1, 2, 62, 9, 17, 17, 1);
INSERT INTO nsd VALUES ('АКО п', 1, 6, 1, 2, 49, 2, 2, 13, 1);
INSERT INTO nsd VALUES ('АКО з', 1, 6, 1, 2, 50, 2, 3, 13, 1);
INSERT INTO nsd VALUES ('КО ЛВС в', 34, 4, 1, 2, 35, 35, 69, 6, 1);
INSERT INTO nsd VALUES ('АРПУ н', 33, 6, 1, 2, 34, 34, 66, 6, 2);
INSERT INTO nsd VALUES ('АРПУ в ', 33, 6, 1, 2, 33, 34, 67, 6, 3);
INSERT INTO nsd VALUES ('ЯШ131 ОКС БИ п', 37, 6, 1, 2, 39, 38, 74, 8, 2);
INSERT INTO nsd VALUES ('К05М 13-16 в', 24, 6, 1, 2, 16, 25, 49, 1, 1);
INSERT INTO nsd VALUES ('ЯШ131 ОКС БИ з', 37, 6, 1, 2, 40, 38, 75, 8, 3);
INSERT INTO nsd VALUES ('ЯШ136-1 Эл. пит. п', 38, 6, 1, 2, 41, 39, 76, 8, 0);
INSERT INTO nsd VALUES ('ЯШ136-1 Эл. пит. з', 38, 6, 1, 2, 42, 39, 77, 8, 1);
INSERT INTO nsd VALUES ('К05М 1-4 н', 21, 6, 1, 2, 9, 22, 42, 0, 2);
INSERT INTO nsd VALUES ('К05М 37-40 в', 30, 6, 1, 2, 27, 31, 61, 4, 1);
INSERT INTO nsd VALUES ('К05М 33-36 в', 29, 6, 1, 2, 25, 30, 59, 4, 3);
INSERT INTO nsd VALUES ('К05М 1-4 в', 21, 6, 2, 2, 10, 22, 43, 0, 3);
INSERT INTO nsd VALUES ('РМО3 п', 16, 6, 1, 2, -1, 17, 32, -1, -1);
INSERT INTO nsd VALUES ('РМО3 з', 16, 6, 1, 2, -1, 17, 33, -1, -1);
INSERT INTO nsd VALUES ('РМО4 п', 17, 6, 1, 2, -1, 18, 34, -1, -1);
INSERT INTO nsd VALUES ('РМО4 з', 17, 6, 1, 2, -1, 18, 35, -1, -1);
INSERT INTO nsd VALUES ('РМИШ п', 35, 4, 1, 2, 43, 36, 70, 7, 2);
INSERT INTO nsd VALUES ('РМИШ з', 35, 4, 1, 2, 44, 36, 71, 7, 3);
INSERT INTO nsd VALUES ('кросс С з', 12, 4, 1, 2, 78, 13, 25, 21, 1);


--
-- Name: change_nsd; Type: TRIGGER; Schema: public; Owner: root
--

CREATE TRIGGER change_nsd
    AFTER INSERT OR DELETE OR UPDATE ON nsd
    FOR EACH ROW
    EXECUTE PROCEDURE nsd_trigger();


--
-- PostgreSQL database dump complete
--

