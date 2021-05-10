# Recommended convention: ZEPHYR_APP := <project() name>.<BOARD>

ZEPHYR_APP_NAME := hello_world

BOARD := nucleo_f401re
ZEPHYR_APP := $(ZEPHYR_APP_NAME).$(BOARD)

include Makefile.defs
$(eval $(call inc_rule,zephyr,$(ZEPHYR_APP)))

# Example: create same app for a different board
# BOARD := nucleo_f767zi
# ZEPHYR_APP := $(ZEPHYR_APP_NAME).$(BOARD)
# $(eval $(call inc_rule,zephyr,$(ZEPHYR_APP)))
