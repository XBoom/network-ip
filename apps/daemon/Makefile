# Find the root directory
ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BuildEnv.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/BuildEnv.mk

# Compiler and flags
CC := gcc
CFLAGS += -Wall -Werror -g -O0
INCLUDES := -I$(ROOTDIR)/include -I$(ROOTDIR)/libs/lib_uev/
LDFLAGS := -L$(ROOTDIR)/libs/lib_uev/ -l_uev

# Target
uev: main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) $(INCLUDES) -H -c $< -o $@

clean:
	rm -f uev *.o