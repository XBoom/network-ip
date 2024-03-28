# Find the root directory
ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BuildEnv.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/BuildEnv.mk

# Compiler and flags
CC := gcc
CFLAGS += -g -O0
# INCLUDES := -I$(ROOTDIR)/include -I$(ROOTDIR)/libs/lib_uev/

LDFLAGS += -L$(ROOTDIR)/libs/lib_uev/ -l_uev
LDFLAGS += -L$(ROOTDIR)/libs/lib_log/ -l_log

# Target
daemon: daemon.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

daemon.o: daemon.c
#	$(CC) $(CFLAGS) $(INCLUDES) -H -c $< -o $@
	$(CC) $(CFLAGS) -H -c $< -o $@

clean:
	rm -f daemon *.o