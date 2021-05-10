THIS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
SUBDIRS := hello_world
include Makefile.defs
$(eval $(call inc_subdir,$(THIS_DIR),$(SUBDIRS)))
