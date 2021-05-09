THIS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
SUBDIRS := libs nucleo zephyr
include Makefile.defs
$(eval $(call inc_subdir,$(THIS_DIR),$(SUBDIRS)))
