$(SELF_BUILD_DIR)/%.o: $(SELF_SRC_DIR)/%.c $(BUILD_USR_INCLUDE_ALL)
	mkdir -p $(dir $@)
	$(KERNEL_CC) -c -o $@ \
		-D MEMORY_LOCATION_KERNEL=$(MEMORY_LOCATION_KERNEL) \
		-D MEMORY_LOCATION_APP=$(MEMORY_LOCATION_APP) \
		$<

$(SELF_BUILD_DIR)/%_asm.o: $(SELF_SRC_DIR)/%.asm
	mkdir -p $(dir $@)
	nasm -o $@ -f elf32 $<

$(SELF_BUILD_DIR)/libprocess: $(SELF_BUILD_ALL_C) $(SELF_BUILD_ALL_ASM)
	ar rc $@ $^
