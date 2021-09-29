C_BIN := timeout_list_test

# "includes"
H_DIRS :=
# "srcs"
C_SRCS := src/timeout_list_test.c

# "deps"
DEPEND := libs/cutils/timeout_list:timeout_list libs/cutils/time:time

LFLAGS += -pthread

include $(shell git rev-parse --show-toplevel)/Makefile.defs
$(eval $(call inc_rule,cbin,$(C_BIN)))
