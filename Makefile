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

sketch: install

all: sketch


#SRC = $(BUILD_PATH)/src/core/Brain.cpp \ $(BUILD_PATH)/src/core/History.cpp
#OBJ = $(patsubst $(BUILD_PATH)/%.c,$(BUILD_PATH)/%.o,$(SRC))
install: $(BUILDDIRS) $(TGT_BIN) $(BUILD_PATH)/main.o $(BUILD_PATH)/breezebb
    
#$(BUILD_PATH)/%.o:%.cpp
#	g++  $(GLOBAL_CXXFLAGS) -c $< -o $@
#	
#%.cpp:
#	g++  $(GLOBAL_CXXFLAGS) -c $< $@


#$(BUILD_PATH)/Brain.o:
#	g++ -o $(BUILD_PATH)/Brain.o -c src/core/Brain.cpp $(GLOBAL_CXXFLAGS)
#$(BUILD_PATH)/History.o:
#	g++ -o $(BUILD_PATH)/History.o -c src/core/History.cpp $(GLOBAL_CXXFLAGS)
#$(BUILD_PATH)/Logger.o:
#	g++ -o $(BUILD_PATH)/Logger.o -c src/core/Logger.cpp $(GLOBAL_CXXFLAGS)
#$(BUILD_PATH)/Processing.o:
#	g++ -o $(BUILD_PATH)/Processing.o -c src/core/Processing.cpp $(GLOBAL_CXXFLAGS)

	#--------------------------------
# TODO : http://stackoverflow.com/questions/15652264/makefile-which-can-generate-all-object-files-in-a-specific-path

clean:
	rm -rf build/*
	rm -f main
	rm -f *.o


help:
	@echo ""
	@echo $(TGT_BIN)
	@echo $(BUILDDIRS)
	@echo "TBC"
