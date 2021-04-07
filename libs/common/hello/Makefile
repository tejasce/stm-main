C_LIB := hello

# "includes"
H_DIRS := inc
# "srcs"
C_SRCS := src/hello.c
# "hdrs"
I_HDRS := inc/hello.h

# "deps"
# no external dependency (optional. stating it as demo)
# DEPEND := libs/common/newlib:new
# DEPEND += libs/common/extras:ext
# DEPEND += libs/common/blabla:bla
DEPEND :=

include Makefile.defs
$(eval $(call inc_rule,clib,$(C_LIB)))
