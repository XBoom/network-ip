#1. 找到根目录
ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BuildEnv.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/BuildEnv.mk


DIR ?= 01	#指定目录

$(info ROOTDIR: $(ROOTDIR))
$(info DIR: $(DIR))

# 使用 wildcard 函数获取 src 目录下所有的 .c 文件的文件名。
src = $(wildcard $(DIR)/*.c)

# 使用 patsubst 函数将 src 目录下的 .c 文件路径替换为 build 目录下的 .o 文件路径，从而生成所有的目标文件
obj = $(patsubst $(DIR)/%.c, %.o, $(src))


CFLAGS += -g -O0

LDFLAGS += -L$(ROOTDIR)/libs/lib_uev/ -l_uev
LDFLAGS += -L$(ROOTDIR)/libs/lib_log/ -l_log

PROGS =	client server

all: ${PROGS}

# %.o: 目标文件名模式，其中 % 匹配任意字符。
# $(DIR)/%.c: 源文件名模式，其中 % 匹配任意字符，$(DIR) 变量代表目录名。
# $(CC): 编译器命令，例如 gcc 或 clang。
# $(CFLAGS): 编译器选项，例如 -O2 或 -g。
# -H: 生成依赖关系文件。
# -c: 只编译，不链接。
# $<: 源文件名，由模式规则自动生成。
# $@: 目标文件名，由模式规则自动生成

%.o: $(DIR)/%.c
	$(CC) $(CFLAGS) -H -c $< -o $@

client: client.o
		${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}

server: server.o
		${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}

.PHONY: clean help
clean:
	rm ${PROGS} ./*.o

help:
	@echo "Usage: make [DIR=<directory>]"
	@echo "  Targets:"
	@echo "    all      - Build all targets"
	@echo "    clean    - Clean up generated files"
	@echo "    help     - Display this help message"