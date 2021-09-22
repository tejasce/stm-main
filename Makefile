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
# Changeset info
#
DIRTY := $(filter X,$(lastword $(shell git describe --tags --all --dirty=' X')))
CHANGESET := $(shell git log -1 --format=%h$(DIRTY))

#
# This repo ships with its build-environment. All the build
# (vs clean etc) targets work as intended inside the buildenv.
# User must first build it and use a "buildenv shell" to build
# source in this repo. All the non-build targets that works
# both in and out of buildenv shell are filtered for this rule.
# One more excpetion is $(flash) flag which is necessary on OSX.
#
ifeq ($(BUILDENV_SHELL)$(flash),)
ifeq ($(filter clean% clobber %env format help%,$(MAKECMDGOALS)),)
$(error Run "make env" first. See "make help")
endif
endif

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
PACKAGES :=
PRODTAR := {}

#
# Define targets for directories at the next level
#
THIS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
SUBDIRS := firmware libs cmds
include $(shell git rev-parse --show-toplevel)/Makefile.defs
$(eval $(call inc_subdir,$(THIS_DIR),$(SUBDIRS)))

#
# Define pkg targets for entries in pkgs.json
#
$(eval $(call add_pkg_tgt))

#
# Common targets (after defining rules for targets at each level)
#
$(sort $(OBJ_SUBDIRS)):
	@mkdir -p $@

#
# Top-level targets that calls targets at lower levels
#
all: $(patsubst %,all.%,$(PRODUCTS))
tarball: $(patsubst %,tarball.%,$(PRODUCTS))
pkg: $(patsubst %,pkg.%,$(PACKAGES))
ifeq ("$(origin ARCH)","command line")
clean:
	@printf "%$(PCOL)s %s\n" "[RM]" "$(OBJDIR_PREFIX)$(ARCH)"
	$(Q)rm -rf $(OBJDIR_PREFIX)$(ARCH)
else
clean:
	@printf "%$(PCOL)s %s\n" "[RM]" "$(OBJDIR_PREFIX){$(ARCH),firmware}"
	$(Q)rm -rf $(OBJDIR_PREFIX){$(ARCH),firmware}
endif

#
# Apply clang-format to new+modified src files. Always runs inside buildenv shell
#
format: startenv
	$(Q)$(BUILDENV_C) $(TOPDIR)/build/format.sh -m $(if $(DRY_RUN),-n,)

#
# Wrapper useful "clean" targets
#
cleanall:
	@printf "%$(PCOL)s %s\n" "[RM]" "$(OBJDIR_PREFIX)*"
	$(Q)rm -rf $(OBJDIR_PREFIX)*

clobber: cleanall
	@printf "%$(PCOL)s %s\n" "[RM]" "cscope.* tags"
	$(Q)rm -f cscope.* tags

.DEFAULT_GOAL := all

#
# Include helper makefiles
#
include $(Makefile.buildenv)
include $(Makefile.ros)

help:
	@echo "Build Targets"
	@echo "          all: build all $(ARCH) products (default)"
	@echo "        clean: remove all previously built $(ARCH) products"
	@echo "      tarball: create individual tarball of previously built $(ARCH) products"
	@echo "          pkg: create packages as defined in $(PKG_JSON_REL)"
	@echo "     cleanall: remove all products for all targets architectues"
	@echo "      clobber: cleanall + remove cscope/ctags"
	@echo "       format: run 'clang-format' on new+modified files on this branch"
	@echo "         help: show this message"
	@echo "<path>[:prod]: build all $(ARCH) products for <path> and subdirs below"
	@echo "               if specified, build only the product "prod" in the <path>"
	@echo "               Specify \"clean=1\" to clean"
	@echo "               Specify \"tarball=1\" to create tarball of the built product"
	@echo "               Specify \"flash=1\" to program f/w binary (when applicable)"
	@echo "               For example, make libs/common"
	@echo "                            make cmds/common/pb_example:list_people"
	@echo "                            make libs/common clean=1"
	@echo "                            make firmware/zephyr/hello_world tarball=1"
	@echo "                            make firmware/zephyr/hello_world:hello_world.nucleo_f401re flash=1"
	@echo
	@$(MAKE) --no-print-directory help.buildenv
	@echo
	@echo "Commmand-line overrides"
	@echo "         ARCH: build for a target architecture"
	@echo "               Supported: $(SUPPORTED_ARCHS) [Default: $(TARGET_ARCH)]"
	@echo "      VERBOSE: build verbosity"
	@echo "               On/Off if defined/undefined [Default: not defined]"
	@echo "      DRY_RUN: perform dry-run but do not apply changes"
	@echo "               tgts: format"
	@echo "      RELEASE: build optimized artifacts with release profile"
	@echo "               debug build if undefined [Default: not defined]"
	@echo "   DEV_SERIAL: s/n# of a st-link programmer (from \"st-info --probe\")"
	@echo "               Useful when multiple boards are connected [Default: none]"
	@echo
	@echo "Other supported build systems (outside of default build system)"
	@echo "          ROS: make help.ros"
