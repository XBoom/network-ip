ifeq "$(ROOT_DIR)" "" 
export ROOT_DIR=$(shell while true; do if [ -f BuildEnv.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOT_DIR)/BuildEnv.mk

KO_TARGET = dr_packet

C__SOURCES := $(wildcard *.c)
obj-y += $(C__SOURCES:%.c=%.o)

include $(ROOT_DIR)/Rules.make