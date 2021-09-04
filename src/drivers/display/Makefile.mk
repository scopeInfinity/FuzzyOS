$(BUILD_DRIVERS)/display/libtm_bios: $(SRC_DRIVERS)/display/text_mode_bios.c $(SRC_DRIVERS)/display/text_mode_bios.asm $(SRC_DRIVERS)/display/text_mode.h
	# 16 bit mode
	mkdir -p $(BUILD_DRIVERS)/display/
	$(CC) -m16 -fno-pie -c -Isrc -o $(BUILD_DRIVERS)/display/text_mode_bios_c.o $(SRC_DRIVERS)/display/text_mode_bios.c
	$(NASM) -o $(BUILD_DRIVERS)/display/text_mode_bios_asm.o $(SRC_DRIVERS)/display/text_mode_bios.asm
	ar rc 	$@ $(BUILD_DRIVERS)/display/text_mode_bios_c.o $(BUILD_DRIVERS)/display/text_mode_bios_asm.o

$(BUILD_DRIVERS)/display/libtm_vga: $(SRC_DRIVERS)/display/text_mode_vga.c $(SRC_DRIVERS)/display/text_mode_vga.asm $(SRC_DRIVERS)/display/text_mode.h
	mkdir -p $(BUILD_DRIVERS)/display/
	$(CC) -m32 -fno-pie -c -Isrc -o $(BUILD_DRIVERS)/display/text_mode_vga_c.o $(SRC_DRIVERS)/display/text_mode_vga.c
	$(NASM) -o $(BUILD_DRIVERS)/display/text_mode_vga_asm.o $(SRC_DRIVERS)/display/text_mode_vga.asm
	ar rc $@ $(BUILD_DRIVERS)/display/text_mode_vga_c.o $(BUILD_DRIVERS)/display/text_mode_vga_asm.o
