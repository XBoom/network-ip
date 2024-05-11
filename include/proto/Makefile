# Makefile for proto directory
#1. 找到根目录
ifeq "$(ROOT_DIR)" "" 
export ROOT_DIR=$(shell while true; do if [ -f BuildEnv.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOT_DIR)/BuildEnv.mk

# 定义proto 文件的来源位置(解决proto升成的pb-c不在指定文件夹下而是带上了他的目录结构)
CONF_INPUT_DIR := $(PROTO_SRC_DIR)/confproto/
MSG_INPUT_DIR := $(PROTO_SRC_DIR)/msgproto/

# 定义 proto编译的输出位置
CONF_OUTPUT_DIR := $(PROTO_SRC_DIR)/conf/
MSG_OUTPUT_DIR := $(PROTO_SRC_DIR)/msg/

# 定义元文件
CONF_PROTO_FILES := $(wildcard $(PROTO_SRC_DIR)/confproto/*.proto)
MSG_PROTO_FILES := $(wildcard $(PROTO_SRC_DIR)/msgproto/*.proto)

CONF_C_FILES := $(patsubst $(CONF_INPUT_DIR)/%.proto,$(CONF_OUTPUT_DIR)/%.pb-c.c,$(CONF_PROTO_FILES))
CONF_H_FILES := $(patsubst $(CONF_INPUT_DIR)/%.proto,$(CONF_OUTPUT_DIR)/%.pb-c.h,$(CONF_PROTO_FILES))
MSG_C_FILES := $(patsubst $(MSG_INPUT_DIR)/%.proto,$(MSG_OUTPUT_DIR)/%.pb-c.c,$(MSG_PROTO_FILES))
MSG_H_FILES := $(patsubst $(MSG_INPUT_DIR)/%.proto,$(MSG_OUTPUT_DIR)/%.pb-c.h,$(MSG_PROTO_FILES))

.PHONY: proto clean

#编译proto流程
#1. 给编译工具权限
#2. 构建目录(如果没有)
#3. 清理掉已经编译的内容(如果有)
#3. 编译不同类型消息的目录(global conf msg)
proto:
	@chmod +x $(PROTOC)
	@-mkdir -p $(CONF_OUTPUT_DIR)
	@-mkdir -p $(MSG_OUTPUT_DIR)
	@$(PROTOC) --c_out=$(CONF_OUTPUT_DIR) --proto_path=$(CONF_INPUT_DIR) $(CONF_PROTO_FILES)
	@$(PROTOC) --c_out=$(MSG_OUTPUT_DIR) --proto_path=$(MSG_INPUT_DIR) $(MSG_PROTO_FILES)

# Define the clean target
clean:
	-rm $(MSG_OUTPUT_DIR)*.pb-c.c
	-rm $(MSG_OUTPUT_DIR)*.pb-c.h
	-rm $(CONF_OUTPUT_DIR)*.pb-c.c
	-rm $(CONF_OUTPUT_DIR)*.pb-c.h