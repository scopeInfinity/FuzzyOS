external: $(BUILD_DIR)/external/bin/mkfs.ffs $(MINIMAL_DISK)

$(BUILD_DIR)/external/bin/mkfs.ffs: external/src/mkfs_ffs.c
	mkdir -p $(dir $@)
	$(HOST_CC) -o $@ $<

$(BUILD_DIR)/external/bin/mbr_builder: external/src/mbr_builder.c
	mkdir -p $(dir $@)
	$(HOST_CC) -o $@ $<

$(BUILD_DIR)/external/out/fuzzy_mount: $(patsubst $(SRC_APP)/%.c,$(BUILD_APP)/%.out,$(shell find $(SRC_APP)/ -name '*.c'))
    # not a mount
	mkdir -p $@
	cp -f -t $@ $^

$(MINIMAL_DISK): $(BUILD_DIR)/external/bin/mkfs.ffs $(BUILD_DIR)/external/out/fuzzy_mount
	mkdir -p $(dir $@)
	$< $(BUILD_DIR)/external/out/fuzzy_mount/ $@
