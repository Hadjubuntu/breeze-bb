# Try "make help" first

.DEFAULT_GOAL := sketch

##
## Useful paths, constants, etc.
##
SRCROOT := .
BUILD_PATH = build
# Support files for this Makefile
MAKEDIR := $(SRCROOT)/make


##
## Build rules and useful templates
##
include $(MAKEDIR)/build-rules.mk
include $(MAKEDIR)/build-templates.mk

##
## Compilation flags
##
#TARGET_FLAGS    += -I$(LIBMAPLE_PATH)/include/libmaple                       \
#                   -I$(WIRISH_PATH)/include/wirish
GLOBAL_CFLAGS   := -Os -g3 
#-gdwarf-2 -nostdlib \
                   -ffunction-sections -fdata-sections \
		   -Wl,--gc-sections $(TARGET_FLAGS)
GLOBAL_CXXFLAGS := -std=c++11
# -fno-rtti -fno-exceptions -Wall $(TARGET_FLAGS)
GLOBAL_ASFLAGS  := -x assembler-with-cpp $(TARGET_FLAGS)

##
## Set all submodules here
##

#Own Breeze librairies
#LIBMAPLE_MODULES += $(SRCROOT)/src/math/common
#LIBMAPLE_MODULES += $(SRCROOT)/src/math/pid
#LIBMAPLE_MODULES += $(SRCROOT)/src/math/time
#LIBMAPLE_MODULES += $(SRCROOT)/src/math/vector
#LIBMAPLE_MODULES += $(SRCROOT)/src/math/filter
#LIBMAPLE_MODULES += $(SRCROOT)/src/data/conf
#LIBMAPLE_MODULES += $(SRCROOT)/src/hal
#LIBMAPLE_MODULES += $(SRCROOT)/src/peripherals/I2C
#LIBMAPLE_MODULES += $(SRCROOT)/src/peripherals/sensor
#LIBMAPLE_MODULES += $(SRCROOT)/src/peripherals/IMU
#LIBMAPLE_MODULES += $(SRCROOT)/src/processing/ahrs
#LIBMAPLE_MODULES += $(SRCROOT)/src/processing/actuator
#LIBMAPLE_MODULES += $(SRCROOT)/src/processing/flightstabilization
#LIBMAPLE_MODULES += $(SRCROOT)/src/processing/link
#LIBMAPLE_MODULES += $(SRCROOT)/src/processing/nav/sonar
#LIBMAPLE_MODULES += $(SRCROOT)/src/core
#LIBMAPLE_MODULES += $(SRCROOT)/src/link

# User modules:
ifneq ($(USER_MODULES),)
LIBMAPLE_MODULES += $(USER_MODULES)
endif

# Call each module's rules.mk:
$(foreach m,$(LIBMAPLE_MODULES),$(eval $(call LIBMAPLE_MODULE_template,$(m))))

##
## Targets
##

# main target
include $(SRCROOT)/build-targets.mk

.PHONY: install sketch clean help


sketch: clean build-check MSG_INFO $(BUILD_PATH)/$(BOARD).bin

all: sketch



install: $(BUILD_PATH)/main.o main
	g++ -o main $(BUILD_PATH)/main.o

clean:
	rm -rf build/*
	rm main


help:
	@echo ""
	@echo "TBC"
