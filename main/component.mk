#
# Main Makefile. This is basically the same as a component makefile.
#
# This Makefile should, at the very least, just include $(SDK_PATH)/make/component.mk. By default,
# this will take the sources in the src/ directory, compile them and link them into
# lib(subdirectory_name).a in the build directory. This behaviour is entirely configurable,
# please read the SDK documents if you need to do this.
#
THISDIR:=$(dir $(abspath $(lastword $(MAKEFILE_LIST))))
DATETIME := $(shell date "+%Y-%b-%d_%H:%M:%S_%Z")

COMPONENT_ADD_INCLUDEDIRS := include
CFLAGS += -DBUID_TIME=\"$(DATETIME)\" -Wno-error=format=

ifneq ("$(wildcard $(THISDIR)/include/user_config.local.h)","")
CFLAGS += -DLOCAL_CONFIG_AVAILABLE
endif

include $(IDF_PATH)/make/component_common.mk
#include $(IDF_PATH)/make/component.mk
