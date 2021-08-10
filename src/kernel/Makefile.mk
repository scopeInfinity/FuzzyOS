MEMORY_LOCATION_APP = 0x20000

debug_kernel: $(kernel_core)
	objdump -b binary -mi386 -Maddr16,data16 -D $<
	xxd $<

$(SELF_BUILD_DIR)/%.o: $(SELF_SRC_DIR)/%.c $(BUILD_USR_INCLUDE_ALL)
	mkdir -p $(dir $@)
	$(KERNEL_CC) -c -o $@ \
		-D RUN_APP_ID=$(RUN_APP_ID) \
		-D SECTOR_START_APP_TTT=$(SECTOR_START_APP_TTT) \
		-D SECTOR_COUNT_APP_TTT=$(SECTOR_COUNT_APP_TTT) \
		-D MEMORY_LOCATION_KERNEL=$(MEMORY_LOCATION_KERNEL) \
		-D MEMORY_LOCATION_APP=$(MEMORY_LOCATION_APP) \
		-D SECTOR_START_APP_CALC=$(SECTOR_START_APP_CALC) \
		-D SECTOR_COUNT_APP_CALC=$(SECTOR_COUNT_APP_CALC) \
		-D SECTOR_START_APP_LS=$(SECTOR_START_APP_LS) \
		-D SECTOR_COUNT_APP_LS=$(SECTOR_COUNT_APP_LS) \
		-D SECTOR_START_APP_CAT=$(SECTOR_START_APP_CAT) \
		-D SECTOR_COUNT_APP_CAT=$(SECTOR_COUNT_APP_CAT) \
		-D SECTOR_START_APP_SH=$(SECTOR_START_APP_SH) \
		-D SECTOR_COUNT_APP_SH=$(SECTOR_COUNT_APP_SH) \
		$<

$(SELF_BUILD_DIR)/%_asm.o: $(SELF_SRC_DIR)/%.asm
	mkdir -p $(dir $@)
	nasm -o $@ -f elf32 -i $(SRC_REALMODE)/ $<

$(kernel_core): $(BUILD_KERNEL)/core_asm.o \
		$(BUILD_KERNEL)/core.o \
		$(BUILD_KERNEL)/interrupts/libinterrupts \
		$(BUILD_KERNEL)/syscall/libsyscall \
		$(BUILD_KERNEL)/process/libprocess \
		$(BUILD_DIR)/memmgr/tables/libgdt \
		$(BUILD_DIR)/fs/libffs \
		$(BUILD_DRIVERS)/keyboard/libkeyboard \
		$(BUILD_DRIVERS)/pic/libpic \
		$(BUILD_LIB_UTILS)/libutils \
		$(BUILD_DRIVERS)/display/libtm_vga \
		$(BUILD_LIB_DS)/libds \
		$(BUILD_DRIVERS)/disk/libdisk \
		$(BUILD_DIR)/real_mode/librealmodeclient \
		$(BUILD_USR_LIB)/libfuzzyc
	mkdir -p $(dir $@)
	$(LD) --oformat binary -m elf_i386 -Ttext 0x0000 -T linker.ld -o $@ $^
	truncate --size=%512 $(kernel_core)