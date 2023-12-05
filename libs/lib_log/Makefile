#1. 找到根目录
ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BuildEnv.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/BuildEnv.mk

#2. 编译目标
SO_TARGET = lib_log.so

#3. 编译标识
EXTRA_CFLAGS = -fPIC -DWNS_USE_SYSTEM
EXTRA_CFLAGS += -g -O0

#4. 引用库
EXTRA_LDFLAGS = -lprotobuf-c -lwns_confhelp -lwns_conf 

#5. 编译文件
C__SOURCES := $(wildcard *.c)
CPPSOURCES := $(wildcard *.cpp)
CXXSOURCES := $(wildcard *.cc)

obj-y += $(C__SOURCES:%.c=%.o)
obj-y += $(CPPSOURCES:%.cpp=%.o)
obj-y += $(CXXSOURCES:%.cc=%.o)

include $(ROOTDIR)/Rules.make