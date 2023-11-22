-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION h1_germdist" to load this file. \quit


CREATE FUNCTION l1f_germdist(text) RETURNS int
AS
'MODULE_PATHNAME',
'l1f_germdist'
    LANGUAGE C IMMUTABLE
               STRICT
               PARALLEL SAFE;

