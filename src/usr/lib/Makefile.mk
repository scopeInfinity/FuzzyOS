SRC_USR_LIB=$(SRC_DIR)/usr/lib
BUILD_USR_LIB=$(BUILD_DIR)/usr/lib

$(BUILD_USR_LIB)/%.o: $(SRC_USR_LIB)/%.c $(BUILD_USR_INCLUDE_ALL)
	mkdir -p $(dir $@)
	# TODO: Clean up sector location
	$(KERNEL_CC) -c -o $@  \
		-D SECTOR_START_APP_LS=$(SECTOR_START_APP_LS) \
		-D SECTOR_COUNT_APP_LS=$(SECTOR_COUNT_APP_LS) \
		$<

$(BUILD_USR_LIB)/%_asm.o: $(SRC_USR_LIB)/%.asm
	mkdir -p $(dir $@)
	nasm -o $@ -f elf32 $<

$(BUILD_USR_LIB)/libfuzzyc:	$(patsubst $(SRC_USR_LIB)/%.c,$(BUILD_USR_LIB)/%.o,$(shell find $(SRC_USR_LIB) -name '*.c')) \
		$(patsubst $(SRC_USR_LIB)/%.asm,$(BUILD_USR_LIB)/%_asm.o,$(shell find $(SRC_USR_LIB) -name '*.asm'))
	ar rc $@ $^
