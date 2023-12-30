#set environment variables for installation
CC := gcc
#INCLUDES := -I$(ROOTDIR)/include

CFLAGS := -I$(ROOTDIR)/include -Wall -Werror -fPIC

# 编译目标
SO_TARGET := 

#共享库在编译的时候移动到指定的目录，方便运行的时候能够找到共享库
PROCESS_LIBS := /home/libs/

# 进程引用位置
LDFLAGS := -Wl,-rpath,$(PROCESS_LIBS)


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


