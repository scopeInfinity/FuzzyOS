$(BUILD_LIB_UTILS)/libutils_16: $(SRC_LIB_UTILS)/basic.c $(SRC_LIB_UTILS)/basic_16.asm $(SRC_LIB_UTILS)/logging.c $(SRC_LIB_UTILS)/output.c $(SRC_LIB_UTILS)/output.h $(SRC_LIB_UTILS)/string.c $(SRC_LIB_UTILS)/string.h $(SRC_LIB_UTILS)/panic.c $(SRC_LIB_UTILS)/panic.h $(SRC_LIB_UTILS)/panic.asm $(SRC_LIB_UTILS)/time.c $(SRC_LIB_UTILS)/time.h $(SRC_LIB_UTILS)/time.asm $(SRC_LIB_UTILS)/color.c $(SRC_LIB_UTILS)/color.h
	mkdir -p $(BUILD_LIB_UTILS)/
	$(CC) -m16 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/basic_16_c.o $(SRC_LIB_UTILS)/basic.c
	nasm -o $(BUILD_LIB_UTILS)/basic_16_asm.o -f elf32 $(SRC_LIB_UTILS)/basic_16.asm
	$(CC) -m16 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/logging_16.o $(SRC_LIB_UTILS)/logging.c
	$(CC) -m16 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/output_16.o $(SRC_LIB_UTILS)/output.c
	$(CC) -m16 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/string_16.o $(SRC_LIB_UTILS)/string.c
	$(CC) -m16 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/color_16.o $(SRC_LIB_UTILS)/color.c
	$(CC) -m16 -fno-pie -c -D__SOURCE_SNAPSHOT__=$(SOURCE_SNAPSHOT) -Isrc -o $(BUILD_LIB_UTILS)/panic_16_c.o $(SRC_LIB_UTILS)/panic.c
	nasm -o $(BUILD_LIB_UTILS)/panic_16_asm.o -f elf32 $(SRC_LIB_UTILS)/panic.asm
	$(CC) -m16 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/time_16_c.o $(SRC_LIB_UTILS)/time.c
	nasm -o $(BUILD_LIB_UTILS)/time_16_asm.o -f elf32 $(SRC_LIB_UTILS)/time.asm
	ar rc $@ $(BUILD_LIB_UTILS)/basic_16_asm.o $(BUILD_LIB_UTILS)/basic_16_c.o $(BUILD_LIB_UTILS)/logging_16.o $(BUILD_LIB_UTILS)/output_16.o $(BUILD_LIB_UTILS)/string_16.o $(BUILD_LIB_UTILS)/color_16.o $(BUILD_LIB_UTILS)/panic_16_c.o $(BUILD_LIB_UTILS)/panic_16_asm.o $(BUILD_LIB_UTILS)/time_16_c.o $(BUILD_LIB_UTILS)/time_16_asm.o

$(BUILD_LIB_UTILS)/libutils: $(SRC_LIB_UTILS)/basic.c $(SRC_LIB_UTILS)/basic.asm $(SRC_LIB_UTILS)/logging.c $(SRC_LIB_UTILS)/output.c $(SRC_LIB_UTILS)/output.h $(SRC_LIB_UTILS)/input.c $(SRC_LIB_UTILS)/input.h $(SRC_LIB_UTILS)/string.c $(SRC_LIB_UTILS)/string.h $(SRC_LIB_UTILS)/panic.c $(SRC_LIB_UTILS)/panic.h $(SRC_LIB_UTILS)/panic.asm $(SRC_LIB_UTILS)/time.c $(SRC_LIB_UTILS)/time.h $(SRC_LIB_UTILS)/time.asm $(SRC_LIB_UTILS)/color.c $(SRC_LIB_UTILS)/color.h
	mkdir -p $(BUILD_LIB_UTILS)/
	$(CC) -m32 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/basic_c.o $(SRC_LIB_UTILS)/basic.c
	nasm -o $(BUILD_LIB_UTILS)/basic_asm.o -f elf32 $(SRC_LIB_UTILS)/basic.asm
	$(CC) -m32 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/logging.o $(SRC_LIB_UTILS)/logging.c
	$(CC) -m32 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/output.o $(SRC_LIB_UTILS)/output.c
	$(CC) -m32 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/input.o $(SRC_LIB_UTILS)/input.c
	$(CC) -m32 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/string.o $(SRC_LIB_UTILS)/string.c
	$(CC) -m32 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/color.o $(SRC_LIB_UTILS)/color.c
	$(CC) -m32 -fno-pie -c -D__SOURCE_SNAPSHOT__=$(SOURCE_SNAPSHOT) -Isrc -o $(BUILD_LIB_UTILS)/panic_c.o $(SRC_LIB_UTILS)/panic.c
	nasm -o $(BUILD_LIB_UTILS)/panic_asm.o -f elf32 $(SRC_LIB_UTILS)/panic.asm
	$(CC) -m32 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/time_c.o $(SRC_LIB_UTILS)/time.c
	nasm -o $(BUILD_LIB_UTILS)/time_asm.o -f elf32 $(SRC_LIB_UTILS)/time.asm
	ar rc $@ $(BUILD_LIB_UTILS)/basic_asm.o $(BUILD_LIB_UTILS)/basic_c.o $(BUILD_LIB_UTILS)/logging.o $(BUILD_LIB_UTILS)/output.o $(BUILD_LIB_UTILS)/input.o $(BUILD_LIB_UTILS)/string.o $(BUILD_LIB_UTILS)/color.o $(BUILD_LIB_UTILS)/panic_c.o $(BUILD_LIB_UTILS)/panic_asm.o $(BUILD_LIB_UTILS)/time_c.o $(BUILD_LIB_UTILS)/time_asm.o
