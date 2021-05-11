THIS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
SUBDIRS := Lidar_Delivery
include $(shell git rev-parse --show-toplevel)/Makefile.defs
$(eval $(call inc_subdir,$(THIS_DIR),$(SUBDIRS)))
