--
-- PostgreSQL database dump
--

-- Dumped from database version 9.5.12
-- Dumped by pg_dump version 9.5.12

SET statement_timeout = 0;
SET lock_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET client_min_messages = warning;
SET row_security = off;

--
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: account_user; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.account_user (
    id integer,
    name_googleplus character varying(30),
    id_gps integer,
    nickname character varying(30)
);


ALTER TABLE public.account_user OWNER TO postgres;

--
-- Name: computer_user; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.computer_user (
    id integer,
    item_id integer[],
    last_start timestamp without time zone,
    mining_gold integer
);


ALTER TABLE public.computer_user OWNER TO postgres;

--
-- Name: game_user; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.game_user (
    id integer,
    btc integer,
    comp_id integer[],
    energy integer,
    last_start_mining timestamp with time zone,
    inventory integer[]
);


ALTER TABLE public.game_user OWNER TO postgres;

--
-- Name: item_user; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.item_user (
    id integer,
    name character varying(30),
    mining_gold integer,
    item_id integer,
    type integer
);


ALTER TABLE public.item_user OWNER TO postgres;

--
-- Data for Name: account_user; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.account_user (id, name_googleplus, id_gps, nickname) FROM stdin;
\.


--
-- Data for Name: computer_user; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.computer_user (id, item_id, last_start, mining_gold) FROM stdin;
\.


--
-- Data for Name: game_user; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.game_user (id, btc, comp_id, energy, last_start_mining, inventory) FROM stdin;
\.


--
-- Data for Name: item_user; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.item_user (id, name, mining_gold, item_id, type) FROM stdin;
\.


--
-- Name: SCHEMA public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


--
-- PostgreSQL database dump complete
--

