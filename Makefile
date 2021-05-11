#
# Common top level values inheried by all makefiles
#
TOPDIR := $(shell git rev-parse --show-toplevel 2>/dev/null)
ifeq ($(TOPDIR),)
$(error "Not a git repository.")
endif
SHELL := $(shell which bash)
TARGET_ARCH := $(shell uname -m)
SUPPORTED_ARCHS := $(TARGET_ARCH) arm
ifeq ($(TARGET_ARCH),x86_64)
SUPPORTED_ARCHS += aarch64
endif
OBJDIR_PREFIX := objs.

#
# Validate cross-compile arch when specified from cmdline
#
ARCH ?= $(TARGET_ARCH)
ifeq ($(filter $(ARCH),$(SUPPORTED_ARCHS)),)
$(error Error: Unsupported ARCH: "$(ARCH)" (Supported: $(SUPPORTED_ARCHS) | Default: $(TARGET_ARCH)))
endif

#
# Variables populated by makefiles at each level that includes Makefile.<rule>
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
$(sort $(OBJ_SUBDIRS)):
	@mkdir -p $@

#
# Top-level targets that calls targets at lower levels
#
all: $(patsubst %,all.%,$(PRODUCTS))
ifeq ("$(origin ARCH)","command line")
clean:
	$(Q)rm -rf $(OBJDIR_PREFIX)$(ARCH)
else
clean: $(patsubst %,clean.%,$(PRODUCTS))
endif

#
# Wrapper useful "clean" targets
#
cleanall:
	@printf "%$(PCOL)s %s\n" "[RM]" "$(OBJDIR_PREFIX)*"
	$(Q)rm -rf $(OBJDIR_PREFIX)*

clobber: cleanall
	$(Q)rm -f cscope.* tags

.DEFAULT_GOAL := all

#
# Include helper makefiles
#
include $(Makefile.buildenv)
include $(Makefile.ros)

help:
	@echo "Build Targets"
	@echo "         all: build all $(ARCH) products (default)"
	@echo "       clean: remove all previously built $(ARCH) products"
	@echo "    cleanall: remove all products for all targets architectues"
	@echo "     clobber: cleanall + remove cscope/ctags"
	@echo "        help: show this message"
	@echo
	@$(MAKE) --no-print-directory help.buildenv
	@echo
	@echo "Commmand-line overrides"
	@echo "        ARCH: build for a target architecture"
	@echo "              Supported: $(SUPPORTED_ARCHS) [Default: $(TARGET_ARCH)]"
	@echo "     VERBOSE: build verbosity"
	@echo "              On/Off if defined/undefined [Default: not defined]"
	@echo
	@echo "Other supported build systems (outside of default build system)"
	@echo "         ROS: make help.ros"

.SILENT:
