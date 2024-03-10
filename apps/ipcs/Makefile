#1. 找到根目录
ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BuildEnv.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/BuildEnv.mk

DIR ?= 01	#指定目录
FILE ?= 02  #指定文件

$(info ROOTDIR: $(ROOTDIR))
$(info DIR/FILE: $(DIR)/$(FILE))

# 使用 wildcard 函数获取 src 目录下所有的 .c 文件的文件名。
src = $(wildcard $(DIR)/*.c)

# 使用 filter 函数过滤出指定文件
filtered_src = $(filter $(FILE).c, $(src))

# 使用 patsubst 函数将 src 目录下的 .c 文件路径替换为 build 目录下的 .o 文件路径，从而生成所有的目标文件
obj = $(patsubst $(DIR)/$(FILE).c, $(FILE).o, $(filtered_src))


CFLAGS += -g -O0

LDFLAGS += -L$(ROOTDIR)/libs/lib_uev/ -l_uev
LDFLAGS += -L$(ROOTDIR)/libs/lib_log/ -l_log

PROGS = demo

all: ${PROGS}

demo.o: $(DIR)/$(FILE).c
	$(CC) $(CFLAGS) -H -c $< -o $@

demo: demo.o
		${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}

.PHONY: clean help
clean:
	rm ${PROGS} ./*.o