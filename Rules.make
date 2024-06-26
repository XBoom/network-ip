# 此文件用来在多个Makefile之间共享规则

# 1. 根据语言设置编译命令
# 如果 VARIABLE 的值等于 value，执行这里的命令
ifeq ($(PREFIX_CC),)
ifeq ($(wildcard *.cpp *.cc),)
    PREFIX_CC=$(CC)
else
    PREFIX_CC=$(CPP)
endif
endif

# 2. 构建递归的子目录列表，后续通过目录中的 subdir-xx 变量控制编译子目录
SUB_DIRS := $(subdir-y)

# 3. 定义了first_rule目标，依赖于sub_dirs目标，
first_rule: sub_dirs
	make all_targets
	
# 递归子目录，通过patsubst函数创建一个以_subdir_为前缀的子目录目标列表，
# 然后定义sub_dirs目标，它依赖于一个名为dummy的伪目标和这些子目录目标
subdir-list = $(patsubst %,_subdir_%,$(SUB_DIRS))
$(info subdir-list: $(subdir-list))
sub_dirs: dummy $(subdir-list)

ifdef SUB_DIRS
$(subdir-list) : dummy
	make -C $(patsubst _subdir_%,%,$@)
endif

all_targets: $(SO_TARGET) $(KO_TARGET) $(E_TARGET) 

#============ 编译共享库 start ===================
ifdef SO_TARGET

# 编译位置无关的共享库
CFLAGS += -fPIC

$(SO_TARGET): $(obj-y)
	$(PREFIX_CC) -shared -o $@ $(filter $(obj-y), $^) $(LDFLAGS) $(EXTRA_LDFLAGS)

#设置默认目标，在执行 make 之后会自动执行
.DEFAULT_GOAL := install

install: $(SO_TARGET)
# 将 .so 文件移动到指定目录
	cp $< $(PROCESS_LIBS)/$<
endif

# 递归的将头文件复制到编译目录include目录下
# ifdef INSTALL_INC
# 	cp -rvf $(INSTALL_INC) $(BUILD_ROOT)/include
# endif
#	mkdir -p $(PREFIX_LIB)/$(BUILD_LIBS_SUBDIR)
#	mkdir -p $(PACKET_ROOT)$(PREFIX_LIB)/$(BUILD_LIBS_SUBDIR)
#	cp $(SO_TARGET) $(PACKET_ROOT)$(PREFIX_LIB)/$(BUILD_LIBS_SUBDIR)
#	cp $(SO_TARGET) $(PREFIX_LIB)/$(BUILD_LIBS_SUBDIR)
# endif
#============ 编译共享库 end ===================

#============ 编译内核模块 start ===================
ifdef KO_TARGET
CFLAGS=
LDFLAGS=
KO_TARGET: 
	make -C $(OS_KERNEL_VERSION) M=$(shell pwd) modules
	mkdir -p $(PACKET_ROOT)$(PREFIX_MODULE)
	CP $(KO_TARGET) $(PACKET_ROOT)$(PREFIX_MODULE)
$(KO_TARGET): KO_TARGET
endif

#============ 编译内核模块 end ===================

#============ 编译进程 start ===================
ifdef E_TARGET
$(E_TARGET): $(obj-y)
	$(info "EXTRA_LDFLAGS" $(EXTRA_LDFLAGS))
	-rm -f $@
	$(PREFIX_CC) -o $@ $(filter $(obj-y), $^) $(LDFLAGS) $(EXTRA_LDFLAGS)
endif
#============ 编译进程 end ===================

# 编译所有目标
# all_targets: $(O_TARGET) $(L_TARGET) $(E_TARGET) $(SO_TARGET) $(KO_TARGET) $(SUP_E_TARGET) $(SUP_SO_TARGET) $(SUP_L_TARGET) $(SUP_O_TARGET) $(PY_TARGET)

#============ 清理所有目标 start ===================
.PHONY = $(clean-subdirs) clean
ifneq ($(subdir-y),)
clean-dirs := $(addprefix _clean_,$(patsubst _subdir_%,%,$(subdir-y)))
$(clean-dirs):
	make -C $(patsubst _clean_%,%,$@) clean
endif

clean-subdirs := $(clean-dirs) dummy
clean: NOTMKDEP=1
clean: $(clean-subdirs)
	-rm -rf $(obj-y:.o=.d) $(obj-y) $(obj-m:.o=.d) $(obj-m) $(E_TARGET) $(SO_TARGET) $(L_TARGET) $(KO_TARGET) obj $(clean-files) *.d *.o *.gcda *.gcno *.bdf xtest/*.o xtest/*.gcda xtest/*.gcno
	-rm -rf $(CLEAN_OBJ)

#============ 清理所有目标 start ===================

# A rule to do nothing
dummy:

# 保护基础变量文件
include $(ROOT_DIR)/BaseVar.mk