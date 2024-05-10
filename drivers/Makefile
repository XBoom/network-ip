ifeq "$(ROOT_DIR)" "" 
export ROOT_DIR=$(shell while true; do if [ -f BuildEnv.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOT_DIR)/BuildEnv.mk

# 定义 ${OPTIONS_LIB_LOG}=y 最后变成 subdir-y += lib_log
subdir-${OPTIONS_DRIVER_PACKET} += dr_packet

include $(ROOT_DIR)/Rules.make