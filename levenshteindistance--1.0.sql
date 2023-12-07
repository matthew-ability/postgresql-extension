-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION levenshteindistance" to load this file. \quit

CREATE FUNCTION levenshteindistance(text, text) RETURNS int
AS
'MODULE_PATHNAME',
'levenshteindistance'
    LANGUAGE C IMMUTABLE
               STRICT
               PARALLEL SAFE;

