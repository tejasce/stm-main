C_LIB := hello_cpp

# "includes"
H_DIRS :=
# "srcs"
C_SRCS := src/hello.cpp
# "hdrs"
I_HDRS := inc/hello.hpp

# "deps"
# no external dependency (optional. stating it as demo)
DEPEND :=

# strip_include_prefix
STRIP_INC_PREFIX := inc
# include_prefix
INC_PREFIX := libhello_cpp

include $(shell git rev-parse --show-toplevel)/Makefile.defs
$(eval $(call inc_rule,clib,$(C_LIB)))
