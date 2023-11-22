# Create my postgres module :)
EXTENSION = hammingdistance
MODULE_big = hammingdistance
OBJS = hammingdistance.o
DATA = *.sql *.control
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs )
include $(PGXS)
