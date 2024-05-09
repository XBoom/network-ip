# 设置不同的编译选项(默认使用MakeOpt_base.mk)的编译选项进行编译
# 如果判断 MakeOpt，默认使用 MakeOpt-Base.mk
ifeq ($(MakeOpt),)
MakeOpt=Base
endif

# 尝试包含一个可选参数
$(info make option: $(MakeOpt))
include $(ROOT_DIR)/MakeOpt-$(MakeOpt).mk

# 设置环境变量
CC := gcc 

CFLAGS := -I$(ROOT_DIR)/include -Wall -Werror -fPIC

PROTO_CFG := $(ROOT_DIR)/include/proto/conf
PROTO_MSG := $(ROOT_DIR)/include/proto/msg

CFLAGS += -I$(PROTO_CFG)
CFLAGS += -I$(PROTO_MSG)

PROTO_SRC := $(ROOT_DIR)/include/proto/

# 编译目标
SO_TARGET := 

#共享库在编译的时候移动到指定的目录，方便运行的时候能够找到共享库
PROCESS_LIBS := /home/libs/

# 进程引用位置
LDFLAGS := -Wl,-rpath,$(PROCESS_LIBS)
LDFLAGS += -lprotobuf-c # proto 库


# 定时 proto 编译器
PROTOC := protoc

# proto 位置
PROTO_SRC_DIR := $(ROOT_DIR)/include/proto

# ifdef COMENT
# $^ 表示规则的所有先决条件（依赖项）,用于指定用于构建目标的所有源文件或依赖项。
# 目标: 依赖项1.c 依赖项2.c
#     gcc $^ -o $@
# 这里的 $^ 会展开为 依赖项1.c 依赖项2.c。


# $@  表示规则的目标。用于指定规则生成的目标文件的名称
# 目标: 依赖项1.c 依赖项2.c
#     gcc $^ -o $@
# 这里的 $@ 会展开为 目标

# $< 表示规则的第一个先决条件（依赖项）。用于在只有一个依赖项时特别指定它。
# 目标: 依赖项1.c
#     gcc $< -o $@
# 这里的 $< 会展开为 依赖项1.c。
# endif


