C_LIB := hello

# "includes"
H_DIRS := inc
# "srcs"
C_SRCS := src/hello.c
# "hdrs"
I_HDRS := inc/hello.h

# "deps"
# no external dependency (optional. stating it as demo)
DEPEND :=

# strip_include_prefix
STRIP_INC_PREFIX := inc
# include_prefix
INC_PREFIX := libhello

include Makefile.defs
$(eval $(call inc_rule,clib,$(C_LIB)))
