
$(SELF_BUILD_DIR)/%.o: $(SELF_SRC_DIR)/%.c $(BUILD_USR_INCLUDE_ALL)
	mkdir -p $(dir $@)
	$(KERNEL_CC) -c -o $@ \
		$<

$(SELF_BUILD_DIR)/%_asm.o: $(SELF_SRC_DIR)/%.asm
	mkdir -p $(dir $@)
	$(NASM) -o $@ $<

$(SELF_BUILD_DIR)/libinterrupts: $(SELF_BUILD_ALL_C) $(SELF_BUILD_ALL_ASM)
	ar rc $@ $^
