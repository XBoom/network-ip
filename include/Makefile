# 获取当前编译根目录以及编译环境参数
ifeq "$(ROOT_DIR)" "" 
export ROOT_DIR=$(shell while true; do if [ -f BuildEnv.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOT_DIR)/BuildEnv.mk

# 添加编译proto逻辑
.PHONY: proto

# 假设所有的 .proto 文件都在 proto/ 目录下
PROTO_SOURCES = $(shell find ./proto -name '*.proto')

# 生成的 C 文件的列表
PROTO_C = $(PROTO_SOURCES:.proto=.pb-c.c)

proto: $(PROTO_C)
	protoc --c_out=. --proto_path=$(dir $<) $<

# 清理生成的 C 文件
clean:
	rm -f $(PROTO_C)