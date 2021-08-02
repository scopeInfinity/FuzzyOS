$(BUILD_REALMODE)/static_library: $(SRC_REALMODE)/static_library.asm $(SRC_REALMODE)/stub.asm
	mkdir -p $(BUILD_REALMODE)/
	nasm -o $@ -f bin -i $(SRC_REALMODE)/ $(SRC_REALMODE)/static_library.asm
	truncate --size=%512 $@

$(SELF_BUILD_DIR)/%_asm.o: $(SELF_SRC_DIR)/%.asm $(SELF_INCLUDE_DIR)/%.h
	mkdir -p $(dir $@)
	nasm -o $@ -f elf32 -i $(SRC_REALMODE)/ $<

$(SELF_BUILD_DIR)/librealmodeclient: $(SELF_BUILD_DIR)/client_asm.o
	ar rc $@ $^

