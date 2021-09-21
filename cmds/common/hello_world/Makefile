C_BIN := hello_world

# "includes"
H_DIRS :=
# "srcs"
C_SRCS := src/hello_world.c

# "deps"
DEPEND := libs/common/hello:hello

# override default binary install prefix (/usr/bin) if needed
# BIN_INSTALL_PREFIX := /usr/local/bin

# override default tar install mode (0755) if needed
# TAR_INSTALL_MODE := 4755

include $(shell git rev-parse --show-toplevel)/Makefile.defs
$(eval $(call inc_rule,cbin,$(C_BIN)))
