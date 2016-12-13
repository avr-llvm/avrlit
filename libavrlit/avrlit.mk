# This file is included by makefiles in board specific subdirectories
F_USB        = $(F_CPU)
ARCH         = AVR8
OPTIMIZATION = s
TARGET       = avrlit
LUFA_PATH    = ../lufa/LUFA
SRC          = ../src/avrlit.cpp \
               ../src/serial/serial.cpp
CC_FLAGS     = -I../src
CPP_FLAGS    =
CPP_STANDARD = c++11
LD_FLAGS     =
OBJDIR       = _build

# Some devices (like the Leonardo) require a USB library for serial.
ifeq ($(ENABLE_LUFA), 1)
$(info Enabling the LUFA USB library)

SRC := $(SRC) ../src/usb-cdc.c \
						  ../src/serial/usb.cpp \
						  $(LUFA_SRC_USB_DEVICE) \
              $(LUFA_PATH)/Drivers/USB/Class/Device/CDCClassDevice.c

CC_FLAGS := $(CC_FLAGS) -I../src -DUSE_LUFA_CONFIG_HEADER -DUSE_LUFA
else
SRC := $(SRC) ../src/serial/hardware.cpp
endif

# Default target

lib:

include $(LUFA_PATH)/Build/lufa_core.mk
include $(LUFA_PATH)/Build/lufa_sources.mk
include $(LUFA_PATH)/Build/lufa_build.mk
include $(LUFA_PATH)/Build/lufa_cppcheck.mk

