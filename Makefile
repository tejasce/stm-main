#
# Common top level values inheried by all makefiles
#
TOPDIR := $(shell git rev-parse --show-toplevel 2>/dev/null)
ifeq ($(TOPDIR),)
$(error "Not a git repository.")
endif
TARGET_ARCH := $(shell uname -m)
SUPPORTED_ARCHS := aarch64 x86_64
OBJDIR_PREFIX := objs.

#
# Validate cross-compile arch when specified from cmdline
#
ARCH ?= $(TARGET_ARCH)
ifeq ($(filter $(ARCH),$(SUPPORTED_ARCHS)),)
$(error Error: Unsupported ARCH: "$(ARCH)" (Supported: $(SUPPORTED_ARCHS) | Default: $(TARGET_ARCH)))
endif

#
# Variables populated by makesfiles at each level that includes Makefile.<rule>
#
PRODUCTS :=
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
	$(Q)rm -rf $(OBJDIR_PREFIX)*

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
