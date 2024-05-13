#1. 找到根目录
ifeq "$(ROOT_DIR)" "" 
export ROOT_DIR=$(shell while true; do if [ -f BuildEnv.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOT_DIR)/BuildEnv.mk

# 编译目标
E_TARGET = uev

# 编译选项
CFLAGS += -g -O0

# 引用头文件
CFLAGS += -I.
# 链接动态库
EXTRA_LDFLAGS += -l_log -l_uev -l_conf

C__SOURCES := $(wildcard *.c)

obj-y += $(C__SOURCES:%.c=%.o)

include $(ROOT_DIR)/Rules.make