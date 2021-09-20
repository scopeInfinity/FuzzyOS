# User Applications
$(BUILD_APP)/%_c.o: $(SRC_APP)/%.c $(BUILD_USR_INCLUDE_ALL)
	mkdir -p $(BUILD_APP)
	$(USER_CC) -c -o $@ $<

$(BUILD_APP)/%_cpp.o: $(SRC_APP)/%.cpp $(BUILD_USR_INCLUDE_ALL)
	mkdir -p $(BUILD_APP)
	$(USER_CPP) -c -o $@ $<

$(BUILD_APP)/%.elf: $(BUILD_LIB_APP_ENTRY) $(BUILD_APP)/%_c.o  $(BUILD_USR_LIB)/libfuzzyc
	$(USER_LD) -o $@ $^ $(BUILD_USR_LIB)/libfuzzyc

$(BUILD_APP)/%.elf: $(BUILD_LIB_APP_ENTRY) $(BUILD_APP)/%_cpp.o $(BUILD_USR_LIB)/libfuzzyc $(BUILD_USR_LIB)/libfuzzycpp
	$(USER_LD) -o $@ $^ $(BUILD_USR_LIB)/libfuzzyc $(BUILD_USR_LIB)/libfuzzycpp

$(BUILD_APP)/%: $(BUILD_APP)/%.elf
	$(FLAT_FROM_ELF) $< $@
	truncate --size=%512 $@
