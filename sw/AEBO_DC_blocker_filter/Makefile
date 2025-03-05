# @copyright (C) Syntacore 2017. All rights reserved.
# SCR sample apps
# Makefile

APP += main

ifeq ("$(ENDIANNESS)","BE")
CFLAGS += -DBE
endif

ifeq ("$(ENDIANNESS)","AEBO")
CFLAGS += -DAEBO
endif

APP_SRC += main.c offset_filter.s offset_filter_be.s prand16.c timing.s

INTERNAL_PRINTF=1

COMMON_BASE = common
include $(COMMON_BASE)/common.mk
