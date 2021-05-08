THIS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
SUBDIRS := nucleo libs zephyr
include Makefile.defs
$(eval $(call inc_subdir,$(THIS_DIR),$(SUBDIRS)))
