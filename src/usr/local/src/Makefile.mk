# User Applications
$(BUILD_APP)/%.o: $(SRC_APP)/%.c $(BUILD_USR_INCLUDE_ALL)
	mkdir -p $(BUILD_APP)
	$(USER_CC) -c -o $@ $<

$(BUILD_APP)/%.o: $(SRC_APP)/%.cpp $(BUILD_USR_INCLUDE_ALL)
	mkdir -p $(BUILD_APP)
	$(USER_CPP) -c -o $@ $<

$(BUILD_APP)/%.elf: $(BUILD_LIB_APP_ENTRY) $(BUILD_APP)/%.o  $(BUILD_USR_LIB)/libfuzzyc
	$(USER_LD) -o $@ $^ $(BUILD_USR_LIB)/libfuzzyc

$(BUILD_APP)/%: $(BUILD_APP)/%.elf
	$(FLAT_FROM_ELF) $< $@
	truncate --size=%512 $@
