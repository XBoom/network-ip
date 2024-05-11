# 获取当前编译根目录以及编译环境参数
ifeq "$(ROOT_DIR)" "" 
export ROOT_DIR=$(shell while true; do if [ -f BuildEnv.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOT_DIR)/BuildEnv.mk

# 添加编译proto逻辑
subdir-${OPTIONS_INCLUDE_PROTO} += proto

include $(ROOT_DIR)/Rules.make