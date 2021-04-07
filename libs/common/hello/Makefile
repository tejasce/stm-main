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

# TODO: support "include_prefix" and "strip_include_prefix"

include Makefile.defs
$(eval $(call inc_rule,clib,$(C_LIB)))
