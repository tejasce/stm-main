THIS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
SUBDIRS := hello hello_cpp
include Makefile.defs
$(eval $(call inc_subdir,$(THIS_DIR),$(SUBDIRS)))
