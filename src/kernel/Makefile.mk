MEMORY_LOCATION_APP = 0x20000

debug_kernel: $(kernel_core)
	objdump -b binary -mi386 -Maddr16,data16 -D $<
	xxd $<

$(kernel_core): $(SRC_KERNEL)/core.asm \
		$(SRC_KERNEL)/core.c \
		$(SRC_KERNEL)/essentials.c \
		$(SRC_KERNEL)/interrupts.c \
		$(SRC_KERNEL)/syscall.c \
		$(INCLUDE_DIR)/memmgr/tables/gdt.h \
		$(SRC_MEMMGR)/tables/gdt.c \
		$(SRC_REALMODE)/stub.asm \
		$(SRC_KERNEL)/interrupts.c \
		$(SRC_KERNEL)/interrupts.asm  \
		$(BUILD_DIR)/fs/libffs \
		$(BUILD_KERNEL)/syscall/libsyscall \
		$(BUILD_KERNEL)/interrupts/libinterrupts \
		$(BUILD_KERNEL)/process/libprocess \
		$(BUILD_DIR)/real_mode/librealmodeclient \
		$(SRC_LIB_UTILS)/output.h \
		$(SRC_DRIVERS)/keyboard/keyboard.h \
		$(BUILD_LIB_UTILS)/libutils \
		$(BUILD_DRIVERS)/keyboard/libkeyboard \
		$(BUILD_DRIVERS)/pic/libpic \
		$(BUILD_DRIVERS)/display/libtm_vga \
		$(BUILD_DRIVERS)/disk/libdisk \
		$(BUILD_USR_LIB)/libfuzzyc
	mkdir -p $$(dirname $(kernel_core))
	nasm -o $(BUILD_KERNEL)/core_asm.o -f elf32 -i $(SRC_REALMODE)/ $(SRC_KERNEL)/core.asm
	nasm -o $(BUILD_KERNEL)/interrupts_asm.o -f elf32 $(SRC_KERNEL)/interrupts.asm
	$(KERNEL_CC) -c \
		-D RUN_APP_ID=$(RUN_APP_ID) \
		-D SECTOR_START_APP_TTT=$(SECTOR_START_APP_TTT) \
		-D SECTOR_COUNT_APP_TTT=$(SECTOR_COUNT_APP_TTT) \
		-D MEMORY_LOCATION_KERNEL=$(MEMORY_LOCATION_KERNEL) \
		-D MEMORY_LOCATION_APP=$(MEMORY_LOCATION_APP) \
		-D SECTOR_START_APP_CALC=$(SECTOR_START_APP_CALC) \
		-D SECTOR_COUNT_APP_CALC=$(SECTOR_COUNT_APP_CALC) \
		-D SECTOR_START_APP_LS=$(SECTOR_START_APP_LS) \
		-D SECTOR_COUNT_APP_LS=$(SECTOR_COUNT_APP_LS) \
		-D SECTOR_START_APP_CAT=$(SECTOR_START_APP_CAT) \
		-D SECTOR_COUNT_APP_CAT=$(SECTOR_COUNT_APP_CAT) \
		-D SECTOR_START_APP_SH=$(SECTOR_START_APP_SH) \
		-D SECTOR_COUNT_APP_SH=$(SECTOR_COUNT_APP_SH) \
		-o $(BUILD_KERNEL)/core_c.o $(SRC_KERNEL)/core.c
	$(LD) --oformat binary -m elf_i386 -Ttext 0x0000 -T linker.ld -o $(kernel_core) \
		$(BUILD_KERNEL)/core_asm.o \
		$(BUILD_KERNEL)/core_c.o \
		$(BUILD_KERNEL)/interrupts_asm.o \
		$(BUILD_KERNEL)/syscall/libsyscall \
		$(BUILD_KERNEL)/interrupts/libinterrupts \
		$(BUILD_KERNEL)/process/libprocess \
		$(BUILD_DIR)/fs/libffs \
		$(BUILD_DRIVERS)/keyboard/libkeyboard \
		$(BUILD_DRIVERS)/pic/libpic \
		$(BUILD_LIB_UTILS)/libutils \
		$(BUILD_DRIVERS)/display/libtm_vga \
		$(BUILD_LIB_DS)/libds \
		$(BUILD_DRIVERS)/disk/libdisk \
		$(BUILD_DIR)/real_mode/librealmodeclient \
		$(BUILD_USR_LIB)/libfuzzyc
	truncate --size=%512 $(kernel_core)