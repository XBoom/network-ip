#1. 找到根目录
ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BuildEnv.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/BuildEnv.mk

# -I include 表示将 include 目录添加到头文件搜索路径，以便编译器可以找到头文件
# -Wall 启用所有警告信息。
# -g 生成包含调试信息的可执行文件，以便进行调试。
# -O0 禁用优化，以便生成更容易调试的代码。
CFLAGS += -g -O0

# 使用 wildcard 函数获取 src 目录下所有的 .c 文件的文件名。
src = $(wildcard ./*.c)

# 使用 patsubst 函数将 src 目录下的 .c 文件路径替换为 build 目录下的 .o 文件路径，从而生成所有的目标文件
obj = $(patsubst ./%.c, ./%.o, $(src))

# 使用 wildcard 函数获取 include 目录下所有的 .h 文件的文件名。
#headers = $(wildcard include/*.h)

# euv_bench 是一个目标，它依赖于 $(obj)，表示可执行文件 euv_bench 由所有的目标文件构建而成
# 在执行命令时，$(CC) 表示使用编译器，$(obj) 表示所有的目标文件，-o euv_bench 表示输出文件为 euv_bench。

# 1. 生成可执行文件
# euv_bench: $(obj)
# 	$(CC) $(obj) -o euv_bench

# 2. 生成共享库
# 生成共享库 lib_event.so
SO_TARGET = lib_event.so

$(SO_TARGET): $(obj)
	$(CC) $(obj) -shared -o $(SO_TARGET)

# build/%.o: src/%.c ${headers}:
# build/%.o 是一个模式规则，表示如何将源文件编译成目标文件。
# 	$< 表示规则中的第一个依赖项，即源文件。
#	-c 表示生成目标文件而不链接。
#	-o $@ 表示输出文件的名称，即目标文件。
# build/%.o: src/%.c ${headers}
# 	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

#设置默认目标，在执行 make 之后会自动执行
.DEFAULT_GOAL := install

install: $(SO_TARGET)
# 将 .so 文件移动到指定目录
	cp $< $(PROCESS_LIBS)/$<

.PHONY: clean
clean:
	rm ./*.o $(SO_TARGET)