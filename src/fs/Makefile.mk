$(SELF_BUILD_DIR)/%.o: $(SELF_SRC_DIR)/%.c $(SELF_INCLUDE_DIR)/%.h
	mkdir -p $(dir $@)
	$(KERNEL_CC) -c \
		-o $@ $<

$(SELF_BUILD_DIR)/libffs:	$(patsubst $(SELF_SRC_DIR)/%.c,$(SELF_BUILD_DIR)/%.o,$(wildcard $(SELF_SRC_DIR)/*.c))
	ar rc $@ $^
