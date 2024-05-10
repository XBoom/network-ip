ifeq "$(ROOT_DIR)" "" 
export ROOT_DIR=$(shell while true; do if [ -f BuildEnv.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOT_DIR)/BuildEnv.mk

# 定义 ${OPTIONS_APP_RCU}=y 最后变成 subdir-y
subdir-${OPTIONS_APP_RCU} += rcu
subdir-${OPTIONS_APP_UEV} += rcu

include $(ROOT_DIR)/Rules.make