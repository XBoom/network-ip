# This file contains rules which are shared between multiple Makefiles.


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