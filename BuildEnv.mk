#set environment variables for installation
CC := gcc
CFLAGS := -I$(ROOTDIR)/include -Wall -fPIC

# 编译目标
SO_TARGET := 

#共享库在编译的时候移动到指定的目录，方便运行的时候能够找到共享库
PROCESS_LIBS := /home/libs/

# 进程引用位置
LDFLAGS := -Wl,-rpath,$(PROCESS_LIBS)