external: $(BUILD_DIR)/external/bin/mkfs.ffs $(BUILD_DIR)/external/example/sample_fs

$(BUILD_DIR)/external/bin/mkfs.ffs: external/src/mkfs_ffs.c
	mkdir -p $(dir $@)
	$(HOST_CC) -o $@ $<

$(BUILD_DIR)/external/bin/mbr_builder: external/src/mbr_builder.c
	mkdir -p $(dir $@)
	$(HOST_CC) -o $@ $<

$(BUILD_DIR)/external/out/minimal_fs: $(BUILD_DIR)/external/bin/mkfs.ffs $(wildcard tests/**/*)
	mkdir -p $(dir $@)
	$< tests/ $@

$(BUILD_DIR)/external/example/sample_fs: $(BUILD_DIR)/external/out/minimal_fs $(BUILD_DIR)/external/bin/mkfs.ffs
	mkdir -p $(dir $@)
	echo "TODO: Append new files to filesystem"
	cp -f $< $@
