THIS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

# This should be same as project() name in CMakeLists.txt but can be any name
ZEPHYR_APP := $(shell find $(THIS_DIR) -name CMakeLists.txt -exec sed -n 's/^project(\([\.-_0-9a-zA-Z]\{1,\}\))$$/\1/p' {} \;)

BOARD := nucleo_f401re

include Makefile.defs
$(eval $(call inc_rule,zephyr,$(ZEPHYR_APP)))
