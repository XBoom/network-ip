# This file contains rules which are shared between multiple Makefiles.

first_rule: sub_dirs
	$(MAKE) all_targets
	
# 递归子目录
subdir-list = $(patsubst %,_subdir_%,$(SUB_DIRS))
sub_dirs: dummy $(subdir-list)

all_targets: $(SO_TARGET)

# 编译共享库
ifdef SO_TARGET

# 编译位置无关的共享库
CFLAGS += -fPIC

$(SO_TARGET): $(obj-y)
	-rm -f $@
	$(PREFIX_CC) -shared -o $@ $(filter $(obj-y), $^) $(LDFLAGS) $(EXTRA_LDFLAGS)
endif
# 递归的将头文件复制到编译目录include目录下
ifdef INSTALL_INC
	cp -rvf $(INSTALL_INC) $(BUILD_ROOT)/include
endif
	mkdir -p $(PREFIX_LIB)/$(BUILD_LIBS_SUBDIR)
	mkdir -p $(PACKET_ROOT)$(PREFIX_LIB)/$(BUILD_LIBS_SUBDIR)
	cp $(SO_TARGET) $(PACKET_ROOT)$(PREFIX_LIB)/$(BUILD_LIBS_SUBDIR)
	cp $(SO_TARGET) $(PREFIX_LIB)/$(BUILD_LIBS_SUBDIR)
endif


first_rule: sub_dirs
	$(MAKE) all_targets

# 编译所有目标
all_targets: $(O_TARGET) $(L_TARGET) $(E_TARGET) $(SO_TARGET) $(KO_TARGET) $(SUP_E_TARGET) $(SUP_SO_TARGET) $(SUP_L_TARGET) $(SUP_O_TARGET) $(PY_TARGET)


# 清理所有目标
.PHONY = $(clean-subdirs) clean
ifneq ($(subdir-y),)
clean-dirs      := $(addprefix _clean_,$(patsubst _subdir_%,%,$(subdir-y)))
$(clean-dirs):
	$(MAKE) -C $(patsubst _clean_%,%,$@) clean
endif