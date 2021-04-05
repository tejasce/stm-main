STM32_ELF := lidar_delivery

THIS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
H_DIRS := $(shell find $(THIS_DIR) -name "*.h" -exec dirname {} \+ 2>/dev/null | uniq | sed "s|$(THIS_DIR)||")
C_SRCS := $(shell find $(THIS_DIR) -name "*.c" -printf "%P\n" 2>/dev/null | sed "s|^\./||")
S_SRCS := $(shell find $(THIS_DIR) -name "*.s" -printf "%P\n" 2>/dev/null | sed "s|^\./||")
LD_SRC := LinkerScript.ld

include Makefile.defs
$(eval $(call inc_rule,stm32,$(STM32_ELF)))
