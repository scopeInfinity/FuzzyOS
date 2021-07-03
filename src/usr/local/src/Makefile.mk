app_dashboard = $(BUILD_APP)/dashboard.out

USER_CC = $(CC) -m32 -fno-pie -Isrc --sysroot=$(BUILD_DIR)
USER_LD = $(LD) --oformat binary -m elf_i386 -Ttext 0x0 -T linker.ld

# User Applications
$(BUILD_APP)/%.o: $(SRC_APP)/%.c $(BUILD_USR_INCLUDE_ALL) $(SRC_LIB_UTILS)/output.h $(SRC_LIB_UTILS)/time.h
	mkdir -p $(BUILD_APP)
	$(USER_CC) -c -o $@ $<

# TODO: libfuzzyc and libutils should not be cyclic dependent.
$(BUILD_APP)/%.out: $(BUILD_LIB_APP_ENTRY) $(BUILD_APP)/%.o  $(BUILD_USR_LIB)/libfuzzyc $(BUILD_LIB_UTILS)/libutils $(BUILD_DRIVERS)/display/libtm_vga $(BUILD_LIB_SYSCALL)/libsyscall
	$(USER_LD) -o $@ $^ $(BUILD_USR_LIB)/libfuzzyc
	truncate --size=%512 $@
