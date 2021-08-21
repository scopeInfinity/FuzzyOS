$(SELF_BUILD_DIR)/%.o: $(SELF_SRC_DIR)/%.c $(SELF_INCLUDE_DIR)/%.h
	mkdir -p $(dir $@)
	$(KERNEL_CC) -c -o $@ \
		$<

$(SELF_BUILD_DIR)/libgdt: $(SELF_BUILD_ALL_C)
	ar rc $@ $^
