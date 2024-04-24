ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BuildEnv.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/BuildEnv.mk

# 定义 ${CONFIG_LIBS_LOG}=y 最后变成 subdir-y += lib_log
subdir-${CONFIG_LIBS_LOG} += lib_log

include $(ROOTDIR)/Rules.make