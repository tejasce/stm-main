C_BIN := hello_world_cpp

# "includes"
H_DIRS :=
# "srcs"
C_SRCS := src/hello_world.cpp

# "deps"
DEPEND := libs/common/hello_cpp:hello_cpp

include $(shell git rev-parse --show-toplevel)/Makefile.defs
$(eval $(call inc_rule,cbin,$(C_BIN)))
