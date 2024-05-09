# Find the root directory
ifeq "$(ROOT_DIR)" "" 
export ROOT_DIR=$(shell while true; do if [ -f BuildEnv.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOT_DIR)/BuildEnv.mk

# Compiler and flags
CC := gcc
CFLAGS += -g -O0
# INCLUDES := -I$(ROOT_DIR)/include -I$(ROOT_DIR)/libs/lib_uev/

LDFLAGS += -L$(ROOT_DIR)/libs/lib_uev/ -l_uev
LDFLAGS += -L$(ROOT_DIR)/libs/lib_log/ -l_log

# Target
daemon: daemon.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

daemon.o: daemon.c
#	$(CC) $(CFLAGS) $(INCLUDES) -H -c $< -o $@
	$(CC) $(CFLAGS) -H -c $< -o $@

clean:
	rm -f daemon *.o