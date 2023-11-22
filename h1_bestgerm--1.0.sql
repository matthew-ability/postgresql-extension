-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION h1_germdist" to load this file. \quit


CREATE FUNCTION h1_bestgerm(text) RETURNS text
AS
'MODULE_PATHNAME',
'h1_bestgerm'
    LANGUAGE C IMMUTABLE
               STRICT
               PARALLEL SAFE;

