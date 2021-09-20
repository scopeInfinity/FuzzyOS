SRC_USR_LIB=$(SRC_DIR)/usr/lib
BUILD_USR_LIB=$(BUILD_DIR)/usr/lib

$(BUILD_USR_LIB)/%.o: $(SRC_USR_LIB)/%.c $(BUILD_USR_INCLUDE_ALL)
	mkdir -p $(dir $@)
	$(KERNEL_CC) -c -o $@ $<

$(BUILD_USR_LIB)/%.o: $(SRC_USR_LIB)/%.cpp $(BUILD_USR_INCLUDE_ALL)
	mkdir -p $(dir $@)
	$(KERNEL_CPP) -c -o $@ $<

$(BUILD_USR_LIB)/%_asm.o: $(SRC_USR_LIB)/%.asm
	mkdir -p $(dir $@)
	nasm -o $@ -f elf32 $<

$(BUILD_USR_LIB)/libfuzzyc: $(patsubst $(SRC_USR_LIB)/%.c,$(BUILD_USR_LIB)/%.o,$(shell find $(SRC_USR_LIB) -name '*.c')) \
		$(patsubst $(SRC_USR_LIB)/%.asm,$(BUILD_USR_LIB)/%_asm.o,$(shell find $(SRC_USR_LIB) -name '*.asm'))
	ar rc $@ $^

$(BUILD_USR_LIB)/libfuzzycpp: $(patsubst $(SRC_USR_LIB)/%.cpp,$(BUILD_USR_LIB)/%.o,$(shell find $(SRC_USR_LIB) -name '*.cpp'))
	ar rc $@ $^
