THIS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
SUBDIRS := types
include $(shell git rev-parse --show-toplevel)/Makefile.defs
$(eval $(call inc_subdir,$(THIS_DIR),$(SUBDIRS)))
