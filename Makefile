# Create my postgres module :)
EXTENSION = hammingdistance
MODULE_big = hammingdistance
OBJS = hammingdistance.o
DATA = hammingdistance--1.0.sql

PG_CONFIG = pg_config

PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
