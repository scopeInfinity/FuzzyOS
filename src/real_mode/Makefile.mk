$(BUILD_REALMODE)/static_library: $(SRC_REALMODE)/static_library.asm $(SRC_REALMODE)/stub.asm
	mkdir -p $(BUILD_REALMODE)/
	nasm -o $@ -f bin -i $(SRC_REALMODE)/ $(SRC_REALMODE)/static_library.asm
	truncate --size=%512 $@