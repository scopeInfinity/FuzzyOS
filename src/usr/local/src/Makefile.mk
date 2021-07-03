app_dashboard = $(BUILD_APP)/dashboard.out

# TODO; remove Isrc
USER_CC = $(CC) -m32 -fno-pie -Isrc
USER_LD = $(LD) --oformat binary -m elf_i386 -Ttext 0x0 -T linker.ld


# User Applications
$(BUILD_APP)/%.o: $(SRC_APP)/%.c $(SRC_LIB_UTILS)/output.h $(SRC_LIB_UTILS)/time.h
	mkdir -p $(BUILD_APP)
	$(USER_CC) -c -o $@ $<

$(BUILD_APP)/%.out: $(app_entry) $(BUILD_APP)/%.o $(BUILD_LIB_UTILS)/libutils $(BUILD_DRIVERS)/display/libtm_vga $(BUILD_LIB_SYSCALL)/libsyscall # And dependecies :/
	$(USER_LD) -o $@ $^
	truncate --size=%512 $@
