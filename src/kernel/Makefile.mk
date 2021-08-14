MEMORY_LOCATION_APP = 0x20000

debug_kernel: $(kernel_core)
	objdump -b binary -mi386 -Maddr32,data32 -D $<
	xxd $<

$(SELF_BUILD_DIR)/%.o: $(SELF_SRC_DIR)/%.c $(BUILD_USR_INCLUDE_ALL)
	mkdir -p $(dir $@)
	$(KERNEL_CC) -c -o $@ \
		-D INIT_APPNAME=\"$(INIT_APPNAME)\" \
		-D __SOURCE_SNAPSHOT__=$(SOURCE_SNAPSHOT) \
		-D MEMORY_LOCATION_KERNEL=$(MEMORY_LOCATION_KERNEL) \
		-D MEMORY_LOCATION_APP=$(MEMORY_LOCATION_APP) \
		$<

$(SELF_BUILD_DIR)/%_asm.o: $(SELF_SRC_DIR)/%.asm
	mkdir -p $(dir $@)
	$(NASM) -o $@ -i $(SRC_REALMODE)/ $<

$(kernel_core).elf: $(SELF_BUILD_DIR)/core_asm.o $(SELF_BUILD_DIR)/panic_asm.o $(SELF_BUILD_ALL_C) \
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
	$(KERNEL_LD) -o $@ $^

$(kernel_core): $(kernel_core).elf
	$(FLAT_FROM_ELF) $< $@
	truncate --size=%512 $@
