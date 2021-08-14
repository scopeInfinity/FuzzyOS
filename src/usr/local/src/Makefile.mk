# User Applications
$(BUILD_APP)/%.o: $(SRC_APP)/%.c $(BUILD_USR_INCLUDE_ALL) $(SRC_LIB_UTILS)/output.h $(SRC_LIB_UTILS)/time.h
	mkdir -p $(BUILD_APP)
	$(USER_CC) -c -o $@ $<

# TODO: libfuzzyc and libutils should not be cyclic dependent.
$(BUILD_APP)/%.elf: $(BUILD_LIB_APP_ENTRY) $(BUILD_APP)/%.o  $(BUILD_USR_LIB)/libfuzzyc $(BUILD_LIB_UTILS)/libutils $(BUILD_DRIVERS)/display/libtm_vga
	$(USER_LD) -o $@ $^ $(BUILD_USR_LIB)/libfuzzyc

$(BUILD_APP)/%.out: $(BUILD_APP)/%.elf
	$(FLAT_FROM_ELF) $< $@
	truncate --size=%512 $@
