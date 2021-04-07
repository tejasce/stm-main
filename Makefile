#
# Common top level values inheried by all makefiles
#
TOPDIR := $(shell git rev-parse --show-toplevel 2>/dev/null)
ifeq ($(TOPDIR),)
$(error "Not a git repository.")
endif
TARGET_ARCH := $(shell uname -m)

#
# Variables populated by makesfiles at each level that includes Makefile.<rule>
#
PRODUCTS :=
OBJDIRS :=
OBJ_SUBDIRS :=

#
# Define targets for directories at the next level
#
THIS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
SUBDIRS := firmware libs cmds
include Makefile.defs
$(eval $(call inc_subdir,$(THIS_DIR),$(SUBDIRS)))

#
# Common targets (after defining rules for targets at each level)
#
$(OBJ_SUBDIRS):
	@mkdir -p $@

#
# Top-level targets that calls targets at lower levels
#
all: $(patsubst %,all.%,$(PRODUCTS))
clean: $(patsubst %,clean.%,$(PRODUCTS))
	@rm -rf $(sort $(OBJDIRS))

.DEFAULT_GOAL := all

#
# Include helper makefiles
#
include $(Makefile.buildenv)

help:
	@echo "Build Targets"
	@echo "        all: build all firmware objects (default)"
	@echo "      clean: remove all previously built firmware objects"
	@echo "       help: show this message"
	@echo
	@$(MAKE) --no-print-directory help.buildenv
