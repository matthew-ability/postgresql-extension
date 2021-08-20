-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION hammingdistance" to load this file. \quit

CREATE FUNCTION hammingdistance(text, text) RETURNS int
AS
'MODULE_PATHNAME',
'hammingdistance'
    LANGUAGE C IMMUTABLE
               STRICT
               PARALLEL SAFE;


