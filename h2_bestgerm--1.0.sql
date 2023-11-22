-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION h1_germdist" to load this file. \quit


CREATE FUNCTION h2_bestgerm(text) RETURNS text
AS
'MODULE_PATHNAME',
'h2_bestgerm'
    LANGUAGE C IMMUTABLE
               STRICT
               PARALLEL SAFE;

