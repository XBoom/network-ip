# 这个Makefile的需求是 
# 1. 将 client 以及 server 目录下的内容编译成进程放置到当前当前目录方便运行
# 2. 引用当前目录 fifo.h 的公共头文件
# 3. 引用proto库

# 1. 找到根目录
ifeq "$(ROOT_DIR)" "" 
export ROOT_DIR=$(shell while true; do if [ -f BuildEnv.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOT_DIR)/BuildEnv.mk

TARGET_CLI=sysv_msg_cli
TARGET_SVR=sysv_msg_cli

# 编译目标
.PHONY: all

all: $(TARGET_CLI) $(TARGET_SVR)

CLIENT_SRC:= $(wildcard ./client/*.c)
SERVER_SRC:= $(wildcard ./server/*.c)

CLIENT_OBJ:= $(CLIENT_SRC:%.c=%.o)
SERVER_OBJ:= $(SERVER_SRC:%.c=%.o)

$(info CLIENT_SRC $(CLIENT_SRC))
$(info SERVER_SRC $(SERVER_SRC))

# 编译选项
CFLAGS += -g -O0

# 引用头文件
CFLAGS += -I.

# 链接动态库
EXTRA_LDFLAGS += -l_log -l_conf

$(TARGET_CLI): $(CLIENT_OBJ)
	@$(PREFIX_CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS)

$(TARGET_SVR): $(SERVER_OBJ)
	@$(PREFIX_CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS)

# 添加自定义清理对象
CLEAN_OBJ+=$(TARGET_CLI)
CLEAN_OBJ+=$(TARGET_SVR)

include $(ROOT_DIR)/Rules.make