THIS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

C_BIN := add_person

# "includes"
H_DIRS :=
# "srcs"
C_SRCS := src/add_person.cc

# "deps"
DEPEND := cmds/common/pb_example/proto:addressbook

CFLAGS += $(PROTOBUF_CFLAGS)
LFLAGS += $(PROTOBUF_LFLAGS)

# Imported from internet: skip linting add_person.cc
NO_LINT := 1

include $(shell git rev-parse --show-toplevel)/Makefile.defs
$(eval $(call inc_rule,cbin,$(C_BIN)))

# add another C_BIN
C_BIN := list_people
H_DIRS :=
C_SRCS := src/list_people.cc
DEPEND := cmds/common/pb_example/proto:addressbook
CFLAGS += $(PROTOBUF_CFLAGS)
LFLAGS += $(PROTOBUF_LFLAGS)

# Imported from internet: skip linting list_people.cc
NO_LINT := 1
$(eval $(call inc_rule,cbin,$(C_BIN)))

# add proto_lib
SUBDIRS := proto
$(eval $(call inc_subdir,$(THIS_DIR),$(SUBDIRS)))
