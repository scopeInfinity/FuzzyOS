$(BUILD_LIB)/app/%.out: $(SRC_LIB)/app/%.asm
	mkdir -p $(BUILD_LIB)/app
	nasm -o $@ -f elf32 $<