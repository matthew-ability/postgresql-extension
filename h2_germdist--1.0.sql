-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION h1_germdist" to load this file. \quit


CREATE FUNCTION h2_germdist(text) RETURNS int
AS
'MODULE_PATHNAME',
'h2_germdist'
    LANGUAGE C IMMUTABLE
               STRICT
               PARALLEL SAFE;

