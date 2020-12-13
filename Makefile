BUILD_DIR = build
SRC_DIR = src
QEMU_SHUT_FLAGS= -no-shutdown -no-reboot
QEMU_EXTRA_FLAGS=
QEMU_GDB_PORT=9000

SRC_BOOTLOADER = $(SRC_DIR)/bootloader
SRC_KERNEL = $(SRC_DIR)/kernel
SRC_DRIVERS = $(SRC_DIR)/drivers
SRC_LIB_DS = $(SRC_DIR)/lib/ds
SRC_LIB_SYSCALL = $(SRC_DIR)/lib/syscall
SRC_LIB_UTILS = $(SRC_DIR)/lib/utils
SRC_LIB = $(SRC_DIR)/lib
SRC_MEMMGR = $(SRC_DIR)/memmgr
SRC_APP = $(SRC_DIR)/app
SRC_REALMODE = $(SRC_DIR)/real_mode

BUILD_BOOTLOADER = $(BUILD_DIR)/bootloader
BUILD_KERNEL = $(BUILD_DIR)/kernel
BUILD_DRIVERS = $(BUILD_DIR)/drivers
BUILD_LIB_DS = $(BUILD_DIR)/lib/ds
BUILD_LIB_SYSCALL = $(BUILD_DIR)/lib/syscall
BUILD_LIB_UTILS = $(BUILD_DIR)/lib/utils
BUILD_LIB = $(BUILD_DIR)/lib
BUILD_APP = $(BUILD_DIR)/app
BUILD_REALMODE = $(BUILD_DIR)/real_mode
.PHONY: all clean

# Files
bt_stage1 = $(BUILD_BOOTLOADER)/stage1
bt_stage2 = $(BUILD_BOOTLOADER)/stage2
image_vmdk = $(BUILD_DIR)/image.vmdk
app_entry = $(BUILD_LIB)/app/entry
rm_static = $(BUILD_REALMODE)/static_library

# Kernel
kernel_core = $(BUILD_DIR)/kernel/core

# Apps
app_calc = $(BUILD_APP)/calc
app_tic_tac_toe = $(BUILD_APP)/tic_tac_toe
app_dashboard = $(BUILD_APP)/dashboard

# Parameters
# 1-byte hex and 1-based indexing for BT STAGE 2 ONLY
SECTOR_START_BT_STAGE2 = 03
SECTOR_COUNT_BT_STAGE2 = 19
SECTOR_START_SHARED_LIBRARY = 1
SECTOR_COUNT_SHARED_LIBRARY = 1
SECTOR_START_KERNEL = 27
SECTOR_COUNT_KERNEL = 33
SECTOR_START_APP_TTT = 60
SECTOR_COUNT_APP_TTT = 25
SECTOR_START_APP_CALC = 85
SECTOR_COUNT_APP_CALC= 25

MEMORY_STATIC_LIBRARY = 0x7E00
MEMORY_LOCATION_KERNEL = 0xC000
MEMORY_LOCATION_APP = 0x20000

SOURCE_SNAPSHOT="\"$$(git rev-parse --short HEAD)$$(git diff --quiet || echo '_unstaged')\""

# General Assumptions
## Integer is 4 bytes

# Tools
CC=gcc -std=c++11 -Os

# Targets
rebuild: clean all_artifacts

test: $(image_vmdk) $(wildcard tests/**/*)
	bash tests/run.sh

all_artifacts: images binaries

images: $(image_vmdk)

binaries: $(bt_stage1) $(bt_stage2) $(kernel_core) $(rm_static)

$(image_vmdk): $(bt_stage1) $(bt_stage2) $(kernel_core) $(app_calc) $(app_tic_tac_toe) $(rm_static)
	dd bs=512 count=2 if=$(bt_stage1) of=$@
	/bin/echo -ne "\x55\xaa" | dd seek=510 bs=1 of=$@
	@echo "Stage 1 Size     : " $$(stat -c %s $(bt_stage1))

	@echo "Static Library"
	@echo "  Got  SECTOR_START_SHARED_LIBRARY  : "$$(( $$(stat -c %s $(image_vmdk)) / 512 ))
	@echo "  Want SECTOR_START_SHARED_LIBRARY  : "$(SECTOR_START_SHARED_LIBRARY)
	cat $(rm_static)  >> $@
	@echo "  Got  SECTOR_COUNT_SHARED_LIBRARY  : "$$(( $$(stat -c %s $(rm_static)) / 512))
	@echo "  Want SECTOR_COUNT_SHARED_LIBRARY  : "$(SECTOR_COUNT_SHARED_LIBRARY)
	@echo "  Size : " $$(stat -c %s $(rm_static))

	@echo "Boot Loader Stage 2"
	@echo "  Got  SECTOR_START_BT_STAGE2   : 0x"$$(printf "%02x\n" $$(( $$(stat -c %s $(image_vmdk)) / 512 + 1)) )
	@echo "  Want SECTOR_START_BT_STAGE2   : 0x"$(SECTOR_START_BT_STAGE2)
	cat $(bt_stage2) >> $@
	@echo "  Got  SECTOR_COUNT_BT_STAGE2   : 0x"$$(printf "%02x\n" $$(( $$(stat -c %s $(bt_stage2)) / 512)) )
	@echo "  Want SECTOR_COUNT_BT_STAGE2   : 0x"$(SECTOR_COUNT_BT_STAGE2)
	@echo "  Size : " $$(stat -c %s $(bt_stage2))

	@echo "Kernel"
	@echo "  Got  SECTOR_START_KERNEL     : "$$(( $$(stat -c %s $(image_vmdk)) / 512 ))
	@echo "  Want SECTOR_START_KERNEL     : "$(SECTOR_START_KERNEL)
	cat $(kernel_core) >> $@
	@echo "  Got  SECTOR_COUNT_KERNEL     : "$$(( $$(stat -c %s $(kernel_core)) / 512))
	@echo "  Want SECTOR_COUNT_KERNEL     : "$(SECTOR_COUNT_KERNEL)
	@echo "  Size : " $$(stat -c %s $(kernel_core))

	@echo "App TicTacToe"
	@echo "  Got  SECTOR_START_APP_TTT    : "$$(( $$(stat -c %s $(image_vmdk)) / 512 ))
	@echo "  Want SECTOR_START_APP_TTT    : "$(SECTOR_START_APP_TTT)
	cat $(app_tic_tac_toe)  >> $@
	@echo "  Got  SECTOR_COUNT_APP_TTT    : "$$(( $$(stat -c %s $(app_tic_tac_toe)) / 512))
	@echo "  Want SECTOR_COUNT_APP_TTT    : "$(SECTOR_COUNT_APP_TTT)
	@echo "  Size    : " $$(stat -c %s $(app_tic_tac_toe))

	@echo "App Calc"
	@echo "  Got  SECTOR_START_APP_CALC    : "$$(( $$(stat -c %s $(image_vmdk)) / 512 ))
	@echo "  Want SECTOR_START_APP_CALC    : "$(SECTOR_START_APP_CALC)
	cat $(app_calc)  >> $@
	@echo "  Got  SECTOR_COUNT_APP_CALC    : "$$(( $$(stat -c %s $(app_calc)) / 512))
	@echo "  Want SECTOR_COUNT_APP_CALC    : "$(SECTOR_COUNT_APP_CALC)
	@echo "  Size    : " $$(stat -c %s $(app_calc))

	@echo "Image Size       : " $$(stat -c %s $@)

debug_stage1: $(bt_stage1)
	objdump -b binary -mi386 -Maddr16,data16 -D $<
	xxd $<

debug_stage2: $(bt_stage2)
	objdump -b binary -mi386 -Maddr16,data16 -D $<
	xxd $<

debug_kernel: $(kernel_core)
	objdump -b binary -mi386 -Maddr16,data16 -D $<
	xxd $<

qemu: $(image_vmdk)
	qemu-system-x86_64 -smp 1 -m 128M -hda $< $(QEMU_SHUT_FLAGS) $(QEMU_EXTRA_FLAGS)

qemu_vvv: $(image_vmdk)
	qemu-system-x86_64 -smp 1 -m 128M -hda $< $(QEMU_SHUT_FLAGS) -d  cpu,exec,in_asm

qemu_debug: $(image_vmdk)
	qemu-system-x86_64 -S -gdb tcp::$(QEMU_GDB_PORT) -smp 1 -m 128M -hda $< $(QEMU_SHUT_FLAGS) -d  cpu,exec,in_asm

qemu_debug_connect:
	gdb -ex "target remote :$(QEMU_GDB_PORT)"

clean:
	rm -r $(BUILD_DIR)/ || echo "Build directory is clean."

# Fuzzy OS
$(bt_stage1): $(SRC_BOOTLOADER)/stage1.asm $(SRC_BOOTLOADER)/constants.asm $(SRC_BOOTLOADER)/io.asm $(SRC_BOOTLOADER)/disk.asm
	mkdir -p $$(dirname $(bt_stage1))
	nasm -o $@ -f bin -i $(SRC_BOOTLOADER)/ -D SECTOR_START_BT_STAGE2=$(SECTOR_START_BT_STAGE2) -D SECTOR_COUNT_BT_STAGE2=$(SECTOR_COUNT_BT_STAGE2) $<
	truncate --size=%512 $@

$(bt_stage2): $(SRC_BOOTLOADER)/stage2.asm $(SRC_BOOTLOADER)/stage2.c $(SRC_MEMMGR)/tables/gdt.c $(SRC_BOOTLOADER)/io.asm $(SRC_BOOTLOADER)/constants.asm $(SRC_REALMODE)/stub.asm $(BUILD_LIB_UTILS)/libutils_16 $(BUILD_DRIVERS)/display/libtm_bios $(BUILD_DRIVERS)/disk/libdisk_16
	mkdir -p $$(dirname $(bt_stage2))
	nasm -o $(BUILD_BOOTLOADER)/stage2_asm.o -f elf32 -i $(SRC_BOOTLOADER)/ -i $(SRC_REALMODE)/ $(SRC_BOOTLOADER)/stage2.asm
	$(CC) -m16 -fno-pie -c -Isrc \
		-D SECTOR_START_SHARED_LIBRARY=$(SECTOR_START_SHARED_LIBRARY) \
		-D SECTOR_COUNT_SHARED_LIBRARY=$(SECTOR_COUNT_SHARED_LIBRARY) \
		-D SECTOR_START_KERNEL=$(SECTOR_START_KERNEL) \
		-D SECTOR_COUNT_KERNEL=$(SECTOR_COUNT_KERNEL) \
		-D MEMORY_STATIC_LIBRARY=$(MEMORY_STATIC_LIBRARY) \
		-D MEMORY_LOCATION_KERNEL=$(MEMORY_LOCATION_KERNEL) \
		-D MEMORY_LOCATION_APP=$(MEMORY_LOCATION_APP) \
		-o $(BUILD_BOOTLOADER)/stage2_c.o $(SRC_BOOTLOADER)/stage2.c
	ld --oformat binary -m elf_i386 -Ttext 0x8000 --strip-all -o $@ $(BUILD_BOOTLOADER)/stage2_asm.o $(BUILD_BOOTLOADER)/stage2_c.o  $(BUILD_LIB_UTILS)/libutils_16 $(BUILD_DRIVERS)/display/libtm_bios $(BUILD_DRIVERS)/disk/libdisk_16
	truncate --size=%512 $@

$(BUILD_REALMODE)/static_library: $(SRC_REALMODE)/static_library.asm $(SRC_REALMODE)/stub.asm
	mkdir -p $(BUILD_REALMODE)/
	nasm -o $@ -f bin -i $(SRC_REALMODE)/ $(SRC_REALMODE)/static_library.asm
	truncate --size=%512 $@

$(kernel_core): $(SRC_KERNEL)/core.asm $(SRC_KERNEL)/core.c $(SRC_KERNEL)/essentials.c $(SRC_KERNEL)/interrupts.c $(SRC_KERNEL)/process.asm $(SRC_KERNEL)/syscall.c $(SRC_MEMMGR)/tables/gdt.c $(SRC_REALMODE)/stub.asm $(SRC_KERNEL)/interrupts.c $(SRC_KERNEL)/interrupts.asm $(SRC_LIB_UTILS)/output.h $(SRC_DRIVERS)/keyboard/keyboard.h $(BUILD_LIB_UTILS)/libutils $(BUILD_DRIVERS)/keyboard/libkeyboard $(BUILD_DRIVERS)/display/libtm_vga $(BUILD_DRIVERS)/disk/libdisk $(BUILD_LIB_SYSCALL)/libsyscall # And other output.h dependecies -_-
	mkdir -p $$(dirname $(kernel_core))
	nasm -o $(BUILD_KERNEL)/core_asm.o -f elf32 -i $(SRC_REALMODE)/ $(SRC_KERNEL)/core.asm
	nasm -o $(BUILD_KERNEL)/process_asm.o -f elf32 -i $(SRC_REALMODE)/ $(SRC_KERNEL)/process.asm
	nasm -o $(BUILD_KERNEL)/interrupts_asm.o -f elf32 $(SRC_KERNEL)/interrupts.asm
	$(CC) -m32 -fno-pie -c -Isrc \
		-D SECTOR_START_APP_TTT=$(SECTOR_START_APP_TTT) \
		-D SECTOR_COUNT_APP_TTT=$(SECTOR_COUNT_APP_TTT) \
		-D MEMORY_LOCATION_KERNEL=$(MEMORY_LOCATION_KERNEL) \
		-D MEMORY_LOCATION_APP=$(MEMORY_LOCATION_APP) \
		-D SECTOR_START_APP_CALC=$(SECTOR_START_APP_CALC) \
		-D SECTOR_COUNT_APP_CALC=$(SECTOR_COUNT_APP_CALC) \
		-o $(BUILD_KERNEL)/core_c.o $(SRC_KERNEL)/core.c
	ld --oformat binary -m elf_i386 --trace -Ttext 0x0000 --strip-all -o $(kernel_core) $(BUILD_KERNEL)/core_asm.o $(BUILD_KERNEL)/process_asm.o $(BUILD_KERNEL)/core_c.o $(BUILD_KERNEL)/interrupts_asm.o $(BUILD_DRIVERS)/keyboard/libkeyboard $(BUILD_LIB_UTILS)/libutils $(BUILD_DRIVERS)/display/libtm_vga $(BUILD_LIB_DS)/libds $(BUILD_DRIVERS)/disk/libdisk $(BUILD_LIB_SYSCALL)/libsyscall
	truncate --size=%512 $(kernel_core)

# Libraries

$(app_entry): $(SRC_LIB)/app/entry.asm
	mkdir -p $$(dirname $(app_entry))
	nasm -o $@ -f elf32 $<

$(BUILD_DRIVERS)/display/libtm_bios: $(SRC_DRIVERS)/display/text_mode_bios.c $(SRC_DRIVERS)/display/text_mode_bios.asm $(SRC_DRIVERS)/display/text_mode.h
	# 16 bit mode
	mkdir -p $(BUILD_DRIVERS)/display/
	$(CC) -m16 -fno-pie -c -Isrc -o $(BUILD_DRIVERS)/display/text_mode_bios_c.o $(SRC_DRIVERS)/display/text_mode_bios.c
	nasm -o $(SRC_DRIVERS)/display/text_mode_bios_asm.o -f elf32 $(SRC_DRIVERS)/display/text_mode_bios.asm
	ar rc 	$@ $(BUILD_DRIVERS)/display/text_mode_bios_c.o $(SRC_DRIVERS)/display/text_mode_bios_asm.o

$(BUILD_DRIVERS)/display/libtm_vga: $(SRC_DRIVERS)/display/text_mode_vga.c $(SRC_DRIVERS)/display/text_mode_vga.asm $(SRC_DRIVERS)/display/text_mode.h
	mkdir -p $(BUILD_DRIVERS)/display/
	$(CC) -m32 -fno-pie -c -Isrc -o $(BUILD_DRIVERS)/display/text_mode_vga_c.o $(SRC_DRIVERS)/display/text_mode_vga.c
	nasm -o $(SRC_DRIVERS)/display/text_mode_vga_asm.o -f elf32 $(SRC_DRIVERS)/display/text_mode_vga.asm
	ar rc $@ $(BUILD_DRIVERS)/display/text_mode_vga_c.o $(SRC_DRIVERS)/display/text_mode_vga_asm.o

$(BUILD_DRIVERS)/keyboard/libkeyboard: $(SRC_DRIVERS)/keyboard/keyboard.c $(SRC_DRIVERS)/keyboard/keyboard.asm $(SRC_DRIVERS)/keyboard/keyboard.h $(SRC_DRIVERS)/keyboard/scancode_handler.c $(SRC_LIB_UTILS)/time.h $(SRC_LIB_DS)/queue.h $(BUILD_LIB_DS)/libds
	mkdir -p $(BUILD_DRIVERS)/keyboard/
	$(CC) -m32 -fno-pie -c -Isrc -o $(BUILD_DRIVERS)/keyboard/keyboard_c.o $(SRC_DRIVERS)/keyboard/keyboard.c
	nasm -o $(BUILD_DRIVERS)/keyboard/keyboard_asm.o -f elf32 $(SRC_DRIVERS)/keyboard/keyboard.asm
	ar rc $@ $(BUILD_DRIVERS)/keyboard/keyboard_c.o $(BUILD_DRIVERS)/keyboard/keyboard_asm.o

$(BUILD_DRIVERS)/disk/libdisk_16: $(SRC_DRIVERS)/disk/disk_16.c $(SRC_DRIVERS)/disk/disk_16.asm $(SRC_DRIVERS)/disk/disk.h
	mkdir -p $(BUILD_DRIVERS)/disk/
	$(CC) -m16 -fno-pie -c -Isrc -o $(BUILD_DRIVERS)/disk/disk_16_c.o $(SRC_DRIVERS)/disk/disk_16.c
	nasm -o $(BUILD_DRIVERS)/disk/disk_16_asm.o -f elf32 $(SRC_DRIVERS)/disk/disk_16.asm
	ar rc $@ $(BUILD_DRIVERS)/disk/disk_16_c.o $(BUILD_DRIVERS)/disk/disk_16_asm.o

$(BUILD_DRIVERS)/disk/libdisk: $(SRC_DRIVERS)/disk/disk.c $(SRC_DRIVERS)/disk/disk.asm $(SRC_DRIVERS)/disk/disk.h $(SRC_REALMODE)/stub.asm
	mkdir -p $(BUILD_DRIVERS)/disk/
	$(CC) -m32 -fno-pie -c -Isrc -o $(BUILD_DRIVERS)/disk/disk_c.o $(SRC_DRIVERS)/disk/disk.c
	nasm -o $(BUILD_DRIVERS)/disk/disk_asm.o -f elf32 -i $(SRC_REALMODE)/ $(SRC_DRIVERS)/disk/disk.asm
	ar rc $@ $(BUILD_DRIVERS)/disk/disk_c.o $(BUILD_DRIVERS)/disk/disk_asm.o

$(BUILD_LIB_UTILS)/libutils_16: $(SRC_LIB_UTILS)/basic.c $(SRC_LIB_UTILS)/basic_16.asm $(SRC_LIB_UTILS)/logging.c $(SRC_LIB_UTILS)/output.c $(SRC_LIB_UTILS)/output.h $(SRC_LIB_UTILS)/string.c $(SRC_LIB_UTILS)/string.h $(SRC_LIB_UTILS)/panic.c $(SRC_LIB_UTILS)/panic.h $(SRC_LIB_UTILS)/panic.asm $(SRC_LIB_UTILS)/time.c $(SRC_LIB_UTILS)/time.h $(SRC_LIB_UTILS)/time.asm $(SRC_LIB_UTILS)/color.c $(SRC_LIB_UTILS)/color.h
	mkdir -p $(BUILD_LIB_UTILS)/
	$(CC) -m16 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/basic_16_c.o $(SRC_LIB_UTILS)/basic.c
	nasm -o $(BUILD_LIB_UTILS)/basic_16_asm.o -f elf32 $(SRC_LIB_UTILS)/basic_16.asm
	$(CC) -m16 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/logging_16.o $(SRC_LIB_UTILS)/logging.c
	$(CC) -m16 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/output_16.o $(SRC_LIB_UTILS)/output.c
	$(CC) -m16 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/string_16.o $(SRC_LIB_UTILS)/string.c
	$(CC) -m16 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/color_16.o $(SRC_LIB_UTILS)/color.c
	$(CC) -m16 -fno-pie -c -D__SOURCE_SNAPSHOT__=$(SOURCE_SNAPSHOT) -Isrc -o $(BUILD_LIB_UTILS)/panic_16_c.o $(SRC_LIB_UTILS)/panic.c
	nasm -o $(BUILD_LIB_UTILS)/panic_16_asm.o -f elf32 $(SRC_LIB_UTILS)/panic.asm
	$(CC) -m16 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/time_16_c.o $(SRC_LIB_UTILS)/time.c
	nasm -o $(BUILD_LIB_UTILS)/time_16_asm.o -f elf32 $(SRC_LIB_UTILS)/time.asm
	ar rc $@ $(BUILD_LIB_UTILS)/basic_16_asm.o $(BUILD_LIB_UTILS)/basic_16_c.o $(BUILD_LIB_UTILS)/logging_16.o $(BUILD_LIB_UTILS)/output_16.o $(BUILD_LIB_UTILS)/string_16.o $(BUILD_LIB_UTILS)/color_16.o $(BUILD_LIB_UTILS)/panic_16_c.o $(BUILD_LIB_UTILS)/panic_16_asm.o $(BUILD_LIB_UTILS)/time_16_c.o $(BUILD_LIB_UTILS)/time_16_asm.o

$(BUILD_LIB_UTILS)/libutils: $(SRC_LIB_UTILS)/basic.c $(SRC_LIB_UTILS)/basic.asm $(SRC_LIB_UTILS)/logging.c $(SRC_LIB_UTILS)/output.c $(SRC_LIB_UTILS)/output.h $(SRC_LIB_UTILS)/input.c $(SRC_LIB_UTILS)/input.h $(SRC_LIB_UTILS)/string.c $(SRC_LIB_UTILS)/string.h $(SRC_LIB_UTILS)/panic.c $(SRC_LIB_UTILS)/panic.h $(SRC_LIB_UTILS)/panic.asm $(SRC_LIB_UTILS)/time.c $(SRC_LIB_UTILS)/time.h $(SRC_LIB_UTILS)/time.asm $(SRC_LIB_UTILS)/color.c $(SRC_LIB_UTILS)/color.h
	mkdir -p $(BUILD_LIB_UTILS)/
	$(CC) -m32 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/basic_c.o $(SRC_LIB_UTILS)/basic.c
	nasm -o $(BUILD_LIB_UTILS)/basic_asm.o -f elf32 $(SRC_LIB_UTILS)/basic.asm
	$(CC) -m32 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/logging.o $(SRC_LIB_UTILS)/logging.c
	$(CC) -m32 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/output.o $(SRC_LIB_UTILS)/output.c
	$(CC) -m32 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/input.o $(SRC_LIB_UTILS)/input.c
	$(CC) -m32 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/string.o $(SRC_LIB_UTILS)/string.c
	$(CC) -m32 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/color.o $(SRC_LIB_UTILS)/color.c
	$(CC) -m32 -fno-pie -c -D__SOURCE_SNAPSHOT__=$(SOURCE_SNAPSHOT) -Isrc -o $(BUILD_LIB_UTILS)/panic_c.o $(SRC_LIB_UTILS)/panic.c
	nasm -o $(BUILD_LIB_UTILS)/panic_asm.o -f elf32 $(SRC_LIB_UTILS)/panic.asm
	$(CC) -m32 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/time_c.o $(SRC_LIB_UTILS)/time.c
	nasm -o $(BUILD_LIB_UTILS)/time_asm.o -f elf32 $(SRC_LIB_UTILS)/time.asm
	ar rc $@ $(BUILD_LIB_UTILS)/basic_asm.o $(BUILD_LIB_UTILS)/basic_c.o $(BUILD_LIB_UTILS)/logging.o $(BUILD_LIB_UTILS)/output.o $(BUILD_LIB_UTILS)/input.o $(BUILD_LIB_UTILS)/string.o $(BUILD_LIB_UTILS)/color.o $(BUILD_LIB_UTILS)/panic_c.o $(BUILD_LIB_UTILS)/panic_asm.o $(BUILD_LIB_UTILS)/time_c.o $(BUILD_LIB_UTILS)/time_asm.o

$(BUILD_LIB_DS)/libds: $(SRC_LIB_DS)/queue.h $(SRC_LIB_DS)/queue.c
	mkdir -p $(BUILD_LIB_DS)/
	$(CC) -m32 -fno-pie -c -Isrc -o $(BUILD_LIB_DS)/queue.o $(SRC_LIB_DS)/queue.c
	ar rc $@ $(BUILD_LIB_DS)/queue.o

$(BUILD_LIB_SYSCALL)/libsyscall: $(SRC_LIB_SYSCALL)/syscall.h $(SRC_LIB_SYSCALL)/syscall.asm
	mkdir -p $(BUILD_LIB_SYSCALL)/
	nasm -o $(BUILD_LIB_SYSCALL)/syscall_asm.o -f elf32 $(SRC_LIB_SYSCALL)/syscall.asm
	ar rc $@ $(BUILD_LIB_SYSCALL)/syscall_asm.o

# User Applications
$(app_calc): $(app_entry) $(SRC_APP)/calc.c $(SRC_LIB_UTILS)/output.h $(SRC_LIB_UTILS)/time.h $(BUILD_LIB_UTILS)/libutils $(BUILD_DRIVERS)/display/libtm_vga $(BUILD_LIB_SYSCALL)/libsyscall # And dependecies :/
	mkdir -p $$(dirname $(app_calc))
	$(CC) -m32 -fno-pie -c -Isrc -o $(BUILD_APP)/calc.o $(SRC_APP)/calc.c
	ld --oformat binary -m elf_i386 -Ttext 0x0 --strip-all -o $@ $(app_entry) $(BUILD_APP)/calc.o $(BUILD_LIB_UTILS)/libutils $(BUILD_DRIVERS)/display/libtm_vga $(BUILD_LIB_SYSCALL)/libsyscall
	truncate --size=%512 $@

$(app_tic_tac_toe): $(app_entry) $(SRC_APP)/tic_tac_toe.c $(SRC_LIB_UTILS)/output.h $(SRC_LIB_UTILS)/input.h $(SRC_LIB_UTILS)/time.h $(BUILD_LIB_UTILS)/libutils $(BUILD_DRIVERS)/display/libtm_vga $(BUILD_LIB_SYSCALL)/libsyscall # And dependecies :/
	mkdir -p $$(dirname $(app_tic_tac_toe))
	$(CC) -m32 -fno-pie -c -Isrc -o $(BUILD_APP)/tic_tac_toe.o $(SRC_APP)/tic_tac_toe.c
	ld --oformat binary -m elf_i386 -Ttext 0x0 --strip-all -o $@ $(app_entry) $(BUILD_APP)/tic_tac_toe.o $(BUILD_LIB_UTILS)/libutils $(BUILD_DRIVERS)/display/libtm_vga $(BUILD_LIB_SYSCALL)/libsyscall
	truncate --size=%512 $@