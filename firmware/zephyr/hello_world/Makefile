THIS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

# This should be same as project() name in CMakeLists.txt but can be any name
ZEPHYR_APP_NAME := hello_world.zapp

BOARD := nucleo_f401re
ZEPHYR_APP := $(ZEPHYR_APP_NAME).$(BOARD)

include Makefile.defs
$(eval $(call inc_rule,zephyr,$(ZEPHYR_APP)))

BOARD := nucleo_f767zi
ZEPHYR_APP := $(ZEPHYR_APP_NAME).$(BOARD)
$(eval $(call inc_rule,zephyr,$(ZEPHYR_APP)))
