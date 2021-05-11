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

include Makefile.defs
$(eval $(call inc_rule,cbin,$(C_BIN)))

# add another C_BIN
C_BIN := list_people
H_DIRS :=
C_SRCS := src/list_people.cc
DEPEND := cmds/common/pb_example/proto:addressbook
CFLAGS += $(PROTOBUF_CFLAGS)
LFLAGS += $(PROTOBUF_LFLAGS)
$(eval $(call inc_rule,cbin,$(C_BIN)))

# add proto_lib
SUBDIRS := proto
$(eval $(call inc_subdir,$(THIS_DIR),$(SUBDIRS)))
