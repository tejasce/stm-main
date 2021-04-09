THIS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
SUBDIRS := hello_world hello_world_cpp
include Makefile.defs
$(eval $(call inc_subdir,$(THIS_DIR),$(SUBDIRS)))
