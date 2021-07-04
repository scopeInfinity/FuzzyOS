SRC_FS=$(SRC_DIR)/fs
BUILD_FS=$(BUILD_DIR)/fs

$(BUILD_FS)/%.o: $(SRC_FS)/%.c $(SRC_FS)/%.h
	mkdir -p $(BUILD_FS)
	$(KERNEL_CC) -c -o $@ $<

$(BUILD_FS)/mkfs.ffs: $(SRC_FS)/mkfs_ffs.c
	mkdir -p $(BUILD_FS)
	$(HOST_CC) -o $@ $<

$(BUILD_FS)/libffs:	$(patsubst $(SRC_FS)/%.c,$(BUILD_FS)/%.o,$(wildcard $(SRC_FS)/*.c))
	ar rc $@ $^
