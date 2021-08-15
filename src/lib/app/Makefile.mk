BUILD_LIB_APP_ENTRY = $(addprefix $(BUILD_LIB)/app/,entry_asm.o)

$(BUILD_LIB)/app/%_asm.o: $(SRC_LIB)/app/%.asm
	mkdir -p $(BUILD_LIB)/app
	nasm -o $@ -f elf32 $<
