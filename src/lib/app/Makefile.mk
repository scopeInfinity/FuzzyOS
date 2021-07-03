BUILD_LIB_APP_ENTRY = $(addprefix $(BUILD_LIB)/app/,entry_asm.o entry.o)

$(BUILD_LIB)/app/%_asm.o: $(SRC_LIB)/app/%.asm
	mkdir -p $(BUILD_LIB)/app
	nasm -o $@ -f elf32 $<

$(BUILD_LIB)/app/%.o: $(SRC_LIB)/app/%.c $(SRC_LIB_UTILS)/output.h $(SRC_LIB_UTILS)/color.h
	mkdir -p $(BUILD_LIB)
	$(KERNEL_CC) -c -o $@ $<
