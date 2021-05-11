C_BIN := hello_world

# "includes"
H_DIRS :=
# "srcs"
C_SRCS := src/hello_world.c

# "deps"
DEPEND := libs/common/hello:hello

include $(shell git rev-parse --show-toplevel)/Makefile.defs
$(eval $(call inc_rule,cbin,$(C_BIN)))
