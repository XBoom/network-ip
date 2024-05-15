# 设置的当前目录
ifeq ($(ROOT_DIR),)
export ROOT_DIR=$(shell while true; do if [ -f BuildEnv.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOT_DIR)/BuildEnv.mk

# 公共与子目录
# DIRS = libs apps
DIRS = include libs drivers apps
# COMMON = include libs tools

# 默认编译目标all
all: subdirs

# PHONY 生命一个伪目标，总是需要执行
.PHONY: subdirs clean

# 子目录
# 变量引用前的$需要用两个$来转义，否则它不会被Makefile解释为变量
subdirs: $(DIRS)
	for dir in $(DIRS) ; do [ ! -d $$dir ] || make -C $$dir || exit 1 ; done

# 安装
install: $(DIRS)
	for dir in $(DIRS) ; do [ ! -d $$dir ] || make -C $$dir install || exit 1 ; done

# 清理
clean:
	echo $(DIRS)
	for dir in $(DIRS) ; do [ ! -d $$dir ] || make -C $$dir clean || exit 1 ; done
# 清理构建目录	rm -rf $(BUILD_ROOT)