
$(BUILD_DRIVERS)/keyboard/libkeyboard: $(SRC_DRIVERS)/keyboard/keyboard.c $(SRC_DRIVERS)/keyboard/keyboard.asm $(SRC_DRIVERS)/keyboard/keyboard.h $(SRC_DRIVERS)/keyboard/scancode_handler.c $(SRC_LIB_UTILS)/time.h $(SRC_LIB_DS)/queue.h $(BUILD_LIB_DS)/libds
	mkdir -p $(BUILD_DRIVERS)/keyboard/
	$(CC) -m32 -fno-pie -c -Isrc -o $(BUILD_DRIVERS)/keyboard/keyboard_c.o $(SRC_DRIVERS)/keyboard/keyboard.c
	nasm -o $(BUILD_DRIVERS)/keyboard/keyboard_asm.o -f elf32 $(SRC_DRIVERS)/keyboard/keyboard.asm
	ar rc $@ $(BUILD_DRIVERS)/keyboard/keyboard_c.o $(BUILD_DRIVERS)/keyboard/keyboard_asm.o
