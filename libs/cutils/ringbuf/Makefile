THIS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

C_LIB := ringbuf

# "includes"
H_DIRS := include
# "srcs"
C_SRCS := src/ringbuf.c
# "hdrs"
I_HDRS := include/ringbuf.h

# "deps"
DEPEND := libs/cutils/alloc:alloc libs/cutils/types:types

# strip_include_prefix
STRIP_INC_PREFIX := include
# include_prefix
INC_PREFIX := cutils

LFLAGS += -pthread

include $(shell git rev-parse --show-toplevel)/Makefile.defs
$(eval $(call inc_rule,clib,$(C_LIB)))
