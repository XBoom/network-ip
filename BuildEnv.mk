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

#共享库在编译的时候移动到指定的目录，方便运行的时候能够找到共享库
PROCESS_LIBS := /home/libs/

# 进程引用位置
# -Wl 允许将参数传递给链接器（ld），链接器通过这些参数来处理链接过程
# -rpath 选项用于指定运行时库搜索路径。
#   例如，-Wl,-rpath,/home/libs/ 将告诉链接器在 /home/libs/ 目录下寻找运行时库
# 
# 使用 -L 指定库文件搜索路径，供链接器查找库文件；而 -rpath 指定程序运行时库搜索路径，用于在运行时加载所需的共享库。
#    这两个选项在链接阶段和程序运行阶段扮演不同的角色，确保程序正确链接和运行所需的库文件
LDFLAGS := -Wl,-rpath,$(PROCESS_LIBS) -L$(PROCESS_LIBS)

# 默认引用库(每个都会引用)
LDFLAGS += -lprotobuf-c # proto 库


# 定时 proto 编译器
PROTOC := $(ROOT_DIR)/tools/protoc

# proto 位置
PROTO_SRC_DIR := $(ROOT_DIR)/include/proto

# 定义proto 文件的来源位置(解决proto升成的pb-c不在指定文件夹下而是带上了他的目录结构)
CONF_INPUT_DIR := $(PROTO_SRC_DIR)/confproto/
MSG_INPUT_DIR := $(PROTO_SRC_DIR)/msgproto/

# 定义 proto编译的输出位置
CONF_OUTPUT_DIR := $(PROTO_SRC_DIR)/conf/
MSG_OUTPUT_DIR := $(PROTO_SRC_DIR)/msg/

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

#内核编译版本
KERNEL_ROOT:=$(ROOT_DIR)/kernel/
OS_KERNEL_VERSION:=$(KERNEL_ROOT)/$(KERNEL_VERSION)

