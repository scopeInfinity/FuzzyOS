$(SELF_BUILD_DIR)/%.o: $(SELF_SRC_DIR)/%.c $(BUILD_USR_INCLUDE_ALL)
	mkdir -p $(dir $@)
	$(KERNEL_CC) -c -o $@ $<

$(SELF_BUILD_DIR)/libsyscall: $(SELF_BUILD_ALL_C)
	ar rc $@ $^
