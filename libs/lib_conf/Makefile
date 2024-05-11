#1. 找到根目录
ifeq "$(ROOT_DIR)" "" 
export ROOT_DIR=$(shell while true; do if [ -f BuildEnv.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOT_DIR)/BuildEnv.mk
# libs详细注释参考lib_log/Makefile

CFLAGS += -g -O0

SO_TARGET = lib_conf.so

proto_src := $(wildcard $(CONF_OUTPUT_DIR)/*.c)
proto_src += $(wildcard $(MSG_OUTPUT_DIR)/*.c)

C__SOURCES := $(proto_src)

obj-y += $(C__SOURCES:%.c=%.o)

include $(ROOT_DIR)/Rules.make