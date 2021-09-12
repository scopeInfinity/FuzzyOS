debug_kernel: $(kernel_core)
	objdump -b binary -mi386 -Maddr32,data32 -D $<
	xxd $<

$(SELF_BUILD_DIR)/%.o: $(SELF_SRC_DIR)/%.c $(BUILD_USR_INCLUDE_ALL)
	mkdir -p $(dir $@)
	$(KERNEL_CC) -c -o $@ \
		-D INIT_APPNAME=\"$(INIT_APPNAME)\" \
		-D __SOURCE_SNAPSHOT__=$(SOURCE_SNAPSHOT) \
		$<

$(SELF_BUILD_DIR)/%_asm.o: $(SELF_SRC_DIR)/%.asm
	mkdir -p $(dir $@)
	$(NASM) -o $@ -i $(SRC_REALMODE)/ $<

$(kernel_core).elf: $(SELF_BUILD_DIR)/core_asm.o \
		$(SELF_BUILD_DIR)/panic_asm.o \
		$(SELF_BUILD_ALL_C) \
		$(BUILD_KERNEL)/interrupts/libinterrupts \
		$(BUILD_KERNEL)/syscall/libsyscall \
		$(BUILD_KERNEL)/process/libprocess \
		$(BUILD_DIR)/memmgr/tables/libgdt \
		$(BUILD_DIR)/fs/libffs \
		$(BUILD_DRIVERS)/ps2/libps2 \
		$(BUILD_DRIVERS)/keyboard/libkeyboard \
		$(BUILD_DRIVERS)/pic/libpic \
		$(BUILD_LIB_UTILS)/libutils \
		$(BUILD_DRIVERS)/display/libtm_vga \
		$(BUILD_DRIVERS)/display/vga/libvga_graphics \
		$(BUILD_LIB_DS)/libds \
		$(BUILD_DRIVERS)/disk/libdisk \
		$(BUILD_DIR)/real_mode/librealmodeclient \
		$(BUILD_USR_LIB)/libfuzzyc \
		$(BUILD_DIR)/memmgr/stackguard/libstackguard # stackguard must be the last one

	mkdir -p $(dir $@)
	$(KERNEL_LD) -o $@ $^

$(kernel_core): $(kernel_core).elf
	$(FLAT_FROM_ELF) $< $@
	truncate --size=%512 $@
