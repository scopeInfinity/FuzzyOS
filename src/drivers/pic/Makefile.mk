$(SELF_BUILD_DIR)/%.o: $(SELF_SRC_DIR)/%.c $(BUILD_USR_INCLUDE_ALL)
	mkdir -p $(dir $@)
	$(KERNEL_CC) -c -o $@ $<

$(SELF_BUILD_DIR)/libpic: $(SELF_BUILD_ALL_C) $(SELF_BUILD_ALL_ASM)
	ar rc $@ $^
