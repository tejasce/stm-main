THIS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

C_LIB := timeout_list

# "includes"
H_DIRS := include
# "srcs"
C_SRCS := src/timeout_list.c
# "hdrs"
I_HDRS := include/timeout_list.h

# "deps"
DEPEND := libs/cutils/alloc:alloc libs/cutils/list:list libs/cutils/time:time  libs/cutils/types:types

# strip_include_prefix
STRIP_INC_PREFIX := include
# include_prefix
INC_PREFIX := cutils

LFLAGS += -pthread

include $(shell git rev-parse --show-toplevel)/Makefile.defs
$(eval $(call inc_rule,clib,$(C_LIB)))

# add test directory
SUBDIRS := test
$(eval $(call inc_subdir,$(THIS_DIR),$(SUBDIRS)))
