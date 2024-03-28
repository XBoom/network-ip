# Makefile for proto directory
#1. 找到根目录
ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BuildEnv.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/BuildEnv.mk

# 定义proto 文件的来源位置(解决proto升成的pb-c不在指定文件夹下而是带上了他的目录结构)
CONF_INPUT_DIR := $(PROTO_SRC_DIR)/confproto
MSG_INPUT_DIR := $(PROTO_SRC_DIR)/msgproto

# 定义 proto编译的输出位置
CONF_OUTPUT_DIR := $(PROTO_SRC_DIR)/conf
MSG_OUTPUT_DIR := $(PROTO_SRC_DIR)/msg

CONF_PROTOC_FLAGS := --c_out=$(CONF_OUTPUT_DIR)
MSG_PROTOC_FLAGS := --c_out=$(MSG_OUTPUT_DIR)

# 定义元文件
CONF_PROTO_FILES := $(wildcard $(PROTO_SRC_DIR)/confproto/*.proto)
MSG_PROTO_FILES := $(wildcard $(PROTO_SRC_DIR)/msgproto/*.proto)

# $(info MSG_PROTO_FILES: $(MSG_PROTO_FILES))
# $(info CONF_PROTO_FILES: $(CONF_PROTO_FILES))

CONF_C_FILES := $(patsubst $(PROTO_SRC_DIR)/confproto/%.proto,$(CONF_OUTPUT_DIR)/%.pb-c.c,$(CONF_PROTO_FILES))
CONF_H_FILES := $(patsubst $(PROTO_SRC_DIR)/confproto/%.proto,$(CONF_OUTPUT_DIR)/%.pb-c.h,$(CONF_PROTO_FILES))
MSG_C_FILES := $(patsubst $(PROTO_SRC_DIR)/msgproto/%.proto,$(MSG_OUTPUT_DIR)/%.pb-c.c,$(MSG_PROTO_FILES))
MSG_H_FILES := $(patsubst $(PROTO_SRC_DIR)/msgproto/%.proto,$(MSG_OUTPUT_DIR)/%.pb-c.h,$(MSG_PROTO_FILES))

# Define the default target
all: conf msg

# Define the target to generate conf files
conf: $(CONF_C_FILES) $(CONF_H_FILES)

# Define the target to generate msg files
msg: $(MSG_C_FILES) $(MSG_H_FILES)

# .PHONY: proto
# proto:
# 执行命令即可
# Define the rule to generate C files from proto files
$(CONF_OUTPUT_DIR)/%.pb-c.c $(CONF_OUTPUT_DIR)/%.pb-c.h: $(PROTO_SRC_DIR)/confproto/%.proto
	@mkdir -p $(CONF_OUTPUT_DIR)
	$(PROTOC) $(CONF_PROTOC_FLAGS) --proto_path=$(CONF_INPUT_DIR) $<

$(MSG_OUTPUT_DIR)/%.pb-c.c $(MSG_OUTPUT_DIR)/%.pb-c.h: $(PROTO_SRC_DIR)/msgproto/%.proto
	@mkdir -p $(MSG_OUTPUT_DIR)
	$(PROTOC) $(MSG_PROTOC_FLAGS) --proto_path=$(MSG_INPUT_DIR) $<



# Define the clean target
clean:
	rm $(CONF_C_FILES) $(CONF_H_FILES) $(MSG_C_FILES) $(MSG_H_FILES)