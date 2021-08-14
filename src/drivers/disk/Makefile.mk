$(BUILD_DRIVERS)/disk/libdisk_16: $(SRC_DRIVERS)/disk/disk_16.c $(SRC_DRIVERS)/disk/disk_16.asm $(SRC_DRIVERS)/disk/disk.h
	mkdir -p $(BUILD_DRIVERS)/disk/
	$(CC) -m16 -fno-pie -c -Isrc -o $(BUILD_DRIVERS)/disk/disk_16_c.o $(SRC_DRIVERS)/disk/disk_16.c
	nasm -o $(BUILD_DRIVERS)/disk/disk_16_asm.o -f elf32 $(SRC_DRIVERS)/disk/disk_16.asm
	ar rc $@ $(BUILD_DRIVERS)/disk/disk_16_c.o $(BUILD_DRIVERS)/disk/disk_16_asm.o

$(BUILD_DRIVERS)/disk/libdisk: $(SRC_DRIVERS)/disk/disk.c $(SRC_DRIVERS)/disk/disk.h
	mkdir -p $(BUILD_DRIVERS)/disk/
	$(KERNEL_CC) -c -o $(BUILD_DRIVERS)/disk/disk_c.o $(SRC_DRIVERS)/disk/disk.c
	ar rc $@ $(BUILD_DRIVERS)/disk/disk_c.o

