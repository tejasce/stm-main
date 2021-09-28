C_LIB := alloc

# "includes"
H_DIRS :=
# "srcs"
C_SRCS :=
# "hdrs"
I_HDRS := include/alloc.h

# "deps"
DEPEND :=

# strip_include_prefix
STRIP_INC_PREFIX := include
# include_prefix
INC_PREFIX := cutils

include $(shell git rev-parse --show-toplevel)/Makefile.defs
$(eval $(call inc_rule,clib,$(C_LIB)))
