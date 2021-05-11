PROTO_LIB := addressbook

PB_SRCS := addressbook.proto

DEPEND :=

include $(shell git rev-parse --show-toplevel)/Makefile.defs
$(eval $(call inc_rule,protobuf,$(PROTO_LIB)))
