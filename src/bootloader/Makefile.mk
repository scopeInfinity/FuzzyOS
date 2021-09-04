MEMORY_STATIC_LIBRARY = 0x7E00

debug_stage1: $(bt_stage1)
	objdump -b binary -mi386 -Maddr16,data16 -D $<
	xxd $<

debug_stage2: $(bt_stage2)
	objdump -b binary -mi386 -Maddr16,data16 -D $<
	xxd $<

$(bt_stage1): $(SRC_BOOTLOADER)/stage1.asm $(SRC_BOOTLOADER)/constants.asm $(SRC_BOOTLOADER)/io.asm $(SRC_BOOTLOADER)/disk.asm
	mkdir -p $$(dirname $(bt_stage1))
	nasm -o $@ -f bin -i $(SRC_BOOTLOADER)/ \
		-D SECTOR_START_BT_STAGE2=$(shell printf "%02x" `expr 1 + $(SECTOR_START_BT_STAGE2)` ) \
		-D SECTOR_COUNT_BT_STAGE2=$(shell printf "%02x" $(SECTOR_COUNT_BT_STAGE2) ) \
		$<
	truncate --size=%512 $@
	@echo $(SECTOR_COUNT_BT_STAGE1)


$(bt_stage2).elf: $(SRC_BOOTLOADER)/stage2.asm $(SRC_BOOTLOADER)/stage2.c $(INCLUDE_DIR)/memmgr/tables/gdt.h $(SRC_MEMMGR)/tables/gdt.c $(SRC_BOOTLOADER)/io.asm $(SRC_BOOTLOADER)/constants.asm $(BUILD_LIB_UTILS)/libutils_16 $(BUILD_DRIVERS)/display/libtm_bios $(BUILD_DRIVERS)/disk/libdisk_16
	mkdir -p $$(dirname $(bt_stage2))
	$(NASM) -o $(BUILD_BOOTLOADER)/stage2_asm.o -i $(SRC_BOOTLOADER)/ $(SRC_BOOTLOADER)/stage2.asm
	$(CC) -m16 -fno-pie -c -Isrc \
	    -Iinclude \
		-D SECTOR_START_SHARED_LIBRARY=$(SECTOR_START_SHARED_LIBRARY) \
		-D SECTOR_COUNT_SHARED_LIBRARY=$(SECTOR_COUNT_SHARED_LIBRARY) \
		-D SECTOR_START_KERNEL=$(SECTOR_START_KERNEL) \
		-D SECTOR_COUNT_KERNEL=$(SECTOR_COUNT_KERNEL) \
		-D MEMORY_STATIC_LIBRARY=$(MEMORY_STATIC_LIBRARY) \
		-o $(BUILD_BOOTLOADER)/stage2_c.o $(SRC_BOOTLOADER)/stage2.c
	$(LD) -m elf_i386 -Ttext 0x8000 -T linker.ld -o $@ $(BUILD_BOOTLOADER)/stage2_asm.o $(BUILD_BOOTLOADER)/stage2_c.o  $(BUILD_LIB_UTILS)/libutils_16 $(BUILD_DRIVERS)/display/libtm_bios $(BUILD_DRIVERS)/disk/libdisk_16
	truncate --size=%512 $@

$(bt_stage2): $(bt_stage2).elf
	$(FLAT_FROM_ELF) $< $@
	truncate --size=%512 $@
