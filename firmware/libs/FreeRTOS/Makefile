THIS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

C_LIB := freertos

# "includes"
H_DIRS := $(shell find $(THIS_DIR) -name "*.h" -exec dirname {} \+ 2>/dev/null | uniq | sed "s|$(THIS_DIR)||")

# "srcs"
C_SRCS := croutine.c list.c queue.c event_groups.c timers.c tasks.c stream_buffer.c portable/GCC/ARM_CM4F/port.c portable/MemMang/heap_1.c

# "hdrs"
I_HDRS := $(shell find $(THIS_DIR)/{include,config} -name "*.h" 2>/dev/null | uniq | sed "s|$(THIS_DIR)/||")

# "deps"
# no external dependency (optional. stating it as demo)
DEPEND :=

# strip_include_prefix
STRIP_INC_PREFIX := include
# include_prefix
INC_PREFIX :=

CFLAGS += $(STM32_CFLAGS)
CFLAGS += -mthumb-interwork -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -std=gnu90 -ffunction-sections -fdata-sections

# Imported from internet: skip linting FreeRTOS
NO_LINT := 1

include $(shell git rev-parse --show-toplevel)/Makefile.defs

# Override to use arm-none-eabi-gcc and, consequently, $(OBJDIR) location
$(eval $(call inc_toolchain,firmware))
$(eval $(call inc_rule,clib,$(C_LIB)))
