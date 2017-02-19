# main project target
$(BUILD_PATH)/main.o: $(SRCROOT)/main.cpp
	$(SILENT_CXX) $(CXX) $(CFLAGS) $(CXXFLAGS) -o $@ -c $< 
$(BUILD_PATH)/SIL.o: $(SRCROOT)/SIL.cpp
	$(SILENT_CXX) $(CXX) $(CFLAGS) $(CXXFLAGS) -o $@ -c $< 

.PHONY: library

$(BUILD_PATH)/breezebb: $(BUILDDIRS) $(TGT_BIN) $(BUILD_PATH)/main.o
	 $(CXX) -o $@ $(TGT_BIN) $(BUILD_PATH)/main.o

$(BUILD_PATH)/SIL: $(BUILDDIRS) $(TGT_BIN) $(BUILD_PATH)/SIL.o
	 $(CXX) -o $@ $(TGT_BIN) $(BUILD_PATH)/SIL.o

$(BUILDDIRS):
	@mkdir -p $@

#$(BUILD_PATH)/$(BOARD).elf: $(BUILDDIRS) $(TGT_BIN) $(BUILD_PATH)/main.o
#	$(SILENT_LD) $(CXX) $(LDFLAGS) -o $@ $(TGT_BIN) $(BUILD_PATH)/main.o -Wl,-Map,$(BUILD_PATH)/$(BOARD).map
#
#$(BUILD_PATH)/$(BOARD).bin: $(BUILD_PATH)/$(BOARD).elf
#	$(SILENT_OBJCOPY) $(OBJCOPY) -v -Obinary $(BUILD_PATH)/$(BOARD).elf $@ 1>/dev/null
#	$(SILENT_DISAS) $(DISAS) -d $(BUILD_PATH)/$(BOARD).elf > $(BUILD_PATH)/$(BOARD).disas
#	@echo " "
#	@echo "Object file sizes:"
#	@find $(BUILD_PATH) -iname *.o | xargs $(SIZE) -t > $(BUILD_PATH)/$(BOARD).sizes
#	@cat $(BUILD_PATH)/$(BOARD).sizes
#	@echo " "
#	@echo "Final Size:"
#	@$(SIZE) $<
#	@echo $(MEMORY_TARGET) > $(BUILD_PATH)/build-type
#
#$(BUILDDIRS):
#	@mkdir -p $@
#
#MSG_INFO:
#	@echo "================================================================================"
#	@echo ""
#	@echo "  Build info:"
#	@echo "     BOARD:          " $(BOARD)
#	@echo "     MCU:            " $(MCU)
#	@echo "     MEMORY_TARGET:  " $(MEMORY_TARGET)
#	@echo ""
#	@echo "  See 'make help' for all possible targets"
#	@echo ""
#	@echo "================================================================================"
#	@echo ""
