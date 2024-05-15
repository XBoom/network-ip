ifeq "$(ROOT_DIR)" "" 
export ROOT_DIR=$(shell while true; do if [ -f BuildEnv.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOT_DIR)/BuildEnv.mk

KO_TARGET = watch_dog.ko

EXTRA_CFLAGS +=

obj-m += main.o

include $(ROOT_DIR)/Rules.make