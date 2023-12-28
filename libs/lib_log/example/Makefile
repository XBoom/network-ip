#1. 找到根目录
ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BuildEnv.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/BuildEnv.mk


# 使用 wildcard 函数获取 src 目录下所有的 .c 文件的文件名。
src = $(wildcard ./*.c)

# 使用 patsubst 函数将 src 目录下的 .c 文件路径替换为 build 目录下的 .o 文件路径，从而生成所有的目标文件
obj = $(patsubst ./%.c, ./%.o, $(src))

# Compiler and flags
CFLAGS += -g -O0

LDFLAGS += -L$(ROOTDIR)/libs/lib_uev/ -l_uev
LDFLAGS += -L$(ROOTDIR)/libs/lib_log/ -l_log

# Target
SO_TARGET = test


$(SO_TARGET): $(obj)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -H -c $< -o $@

clean:
	rm -f $(SO_TARGET) *.o

#include $(ROOTDIR)/Rules.make

# 在程序运行的时候指定库文件搜索路径，可以使用以下几种方法
#1. LD_LIBRARY_PATH 环境变量, 告诉动态链接器在运行时在指定的目录中查找共享库文件
#2.  在编译时使用 -Wl,-rpath 选项指定运行时搜索的库文件路径
# 	LDFLAGS += -Wl,-rpath,/path/to/your/lib
#3. 在运行时设置 LD_PRELOAD:
# 	LD_PRELOAD=/path/to/your/lib/lib_some_library.so ./your_program
