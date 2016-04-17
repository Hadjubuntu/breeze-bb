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
BREEZE_MODULES += $(SRCROOT)/src/math/common
BREEZE_MODULES += $(SRCROOT)/src/math/pid
BREEZE_MODULES += $(SRCROOT)/src/math/time
BREEZE_MODULES += $(SRCROOT)/src/math/vector
BREEZE_MODULES += $(SRCROOT)/src/math/filter
BREEZE_MODULES += $(SRCROOT)/src/data/conf
BREEZE_MODULES += $(SRCROOT)/src/hal
BREEZE_MODULES += $(SRCROOT)/src/peripherals/I2C
BREEZE_MODULES += $(SRCROOT)/src/peripherals/sensor
BREEZE_MODULES += $(SRCROOT)/src/peripherals/IMU
BREEZE_MODULES += $(SRCROOT)/src/processing/ahrs
BREEZE_MODULES += $(SRCROOT)/src/processing/actuator
BREEZE_MODULES += $(SRCROOT)/src/processing/flightstabilization
BREEZE_MODULES += $(SRCROOT)/src/processing/link
BREEZE_MODULES += $(SRCROOT)/src/processing/nav/sonar
BREEZE_MODULES += $(SRCROOT)/src/core
BREEZE_MODULES += $(SRCROOT)/src/link

# User modules:
ifneq ($(USER_MODULES),)
BREEZE_MODULES += $(USER_MODULES)
endif

# Call each module's rules.mk:
$(foreach m,$(BREEZE_MODULES),$(eval $(call LIBMAPLE_MODULE_template,$(m))))

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
