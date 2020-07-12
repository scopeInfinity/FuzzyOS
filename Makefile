BUILD_DIR = build

SRC_BOOTLOADER = src/bootloader
SRC_KERNEL = src/kernel
SRC_DRIVERS = src/drivers
SRC_LIB_UTILS = src/lib/utils
SRC_LIB = src/lib
SRC_APP = src/app

BUILD_BOOTLOADER = build/bootloader
BUILD_KERNEL = build/kernel
BUILD_DRIVERS = build/drivers
BUILD_LIB_UTILS = build/lib/utils
BUILD_LIB = build/lib
BUILD_APP = build/app

.PHONY: all clean

# Files
bt_stage1 = $(BUILD_BOOTLOADER)/stage1
bt_stage2 = $(BUILD_BOOTLOADER)/stage2
image_vmdk = $(BUILD_DIR)/image.vmdk
app_entry = $(BUILD_LIB)/app/entry

# Kernel
kernel_core = $(BUILD_DIR)/kernel/core

# Apps
app_calc = $(BUILD_APP)/calc
app_tick_tac_toe = $(BUILD_APP)/tick_tac_toe
app_dashboard = $(BUILD_APP)/dashboard

# Parameters
BT_STAGE2_SECTOR_COUNT = 19 # In Hex
KERNEL_MEMORY_LOCATION = 0xC000 # 16 bit for now
SOURCE_SNAPSHOT="\"$$(git rev-parse --short HEAD)$$(git diff --quiet || echo '_unstaged')\""

# General Assumptions
## Integer is 4 bytes

rebuild: clean all

all: images binaries

images: $(image_vmdk)

binaries: $(bt_stage1) $(bt_stage2) $(kernel_core)

$(image_vmdk): $(bt_stage1) $(bt_stage2) $(kernel_core) $(app_calc) $(app_tick_tac_toe)
	dd bs=512 count=2 if=$(bt_stage1) of=$@
	/bin/echo -ne "\x55\xaa" | dd seek=510 bs=1 of=$@
	@echo "Stage 1 Size     : " $$(stat -c %s $(bt_stage1))
	@echo "BT_STAGE2_SECTOR_START : "$$(( 1 + $$(stat -c %s $(image_vmdk)) / 512 ))
	cat $(bt_stage2) >> $@
	@echo "Stage 2 Size     : " $$(stat -c %s $(bt_stage2))
	@echo "Want BT_STAGE2_SECTOR_COUNT : 0x"$$(printf "%x\n" $$(( $$(stat -c %s $(bt_stage2)) / 512)) )
	@echo "Got BT_STAGE2_SECTOR_COUNT  : 0x"$(BT_STAGE2_SECTOR_COUNT)

	@echo "AppCalc Sector Start : "$$(( 1 + $$(stat -c %s $(image_vmdk)) / 512 ))
	cat $(app_calc)  >> $@
	@echo "AppCalc Sector Count : "$$(( $$(stat -c %s $(app_calc)) / 512))
	@echo "App Calc Size    : " $$(stat -c %s $(app_calc))

	@echo "App TickTacToe Sector Start : "$$(( 1 + $$(stat -c %s $(image_vmdk)) / 512 ))
	cat $(app_tick_tac_toe)  >> $@
	@echo "App TickTacToe Sector Count : "$$(( $$(stat -c %s $(app_tick_tac_toe)) / 512))
	@echo "App TickTacToe Size    : " $$(stat -c %s $(app_tick_tac_toe))

	@echo "Kernel Core Sector Start : "$$(( 1 + $$(stat -c %s $(image_vmdk)) / 512 ))
	cat $(kernel_core) >> $@
	@echo "Kernel Core Sector Count : "$$(( $$(stat -c %s $(kernel_core)) / 512))
	@echo "Kernel Core Size : " $$(stat -c %s $(kernel_core))
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
	cpulimit -f -l 10 -- qemu-system-x86_64 -smp 1 -m 128M -hda $< -no-shutdown -no-reboot

qemu_debug: $(image_vmdk)
	qemu-system-x86_64 -smp 1 -m 128M -hda $< -no-shutdown -no-reboot -d  cpu,exec,in_asm

clean:
	rm -r $(BUILD_DIR)/ || echo "Build directory is clean."

# Fuzzy OS
$(bt_stage1): $(SRC_BOOTLOADER)/stage1.asm $(SRC_BOOTLOADER)/constants.asm $(SRC_BOOTLOADER)/io.asm $(SRC_BOOTLOADER)/disk.asm
	mkdir -p $$(dirname $(bt_stage1))
	nasm -o $@ -f bin -i $(SRC_BOOTLOADER)/ -D BT_STAGE2_SECTOR_COUNT=$(BT_STAGE2_SECTOR_COUNT) $<
	truncate --size=%512 $@

$(bt_stage2): $(SRC_BOOTLOADER)/stage2.asm $(SRC_BOOTLOADER)/stage2.c $(SRC_BOOTLOADER)/io.asm $(SRC_BOOTLOADER)/constants.asm $(BUILD_LIB_UTILS)/libutils_16 $(BUILD_DRIVERS)/display/libtm_bios
	mkdir -p $$(dirname $(bt_stage2))
	nasm -o $(BUILD_BOOTLOADER)/stage2_asm.o -f elf32 -i $(SRC_BOOTLOADER)/ $(SRC_BOOTLOADER)/stage2.asm
	gcc -m16 -fno-pie -c -Isrc -D KERNEL_MEMORY_LOCATION=$(KERNEL_MEMORY_LOCATION) -o $(BUILD_BOOTLOADER)/stage2_c.o $(SRC_BOOTLOADER)/stage2.c
	ld --oformat binary -m elf_i386 -Ttext 0x8000 --strip-all -o $@ $(BUILD_BOOTLOADER)/stage2_asm.o $(BUILD_BOOTLOADER)/stage2_c.o  $(BUILD_LIB_UTILS)/libutils_16 $(BUILD_DRIVERS)/display/libtm_bios
	truncate --size=%512 $@

$(kernel_core): $(SRC_KERNEL)/core.asm $(SRC_KERNEL)/core.c $(SRC_KERNEL)/essentials.c $(SRC_LIB_UTILS)/io.h $(SRC_DRIVERS)/keyboard/keyboard.h $(BUILD_LIB_UTILS)/libutils $(BUILD_DRIVERS)/keyboard/libkeyboard $(BUILD_DRIVERS)/display/libtm_vga # And other io.h dependecies -_-
	mkdir -p $$(dirname $(kernel_core))
	nasm -o $(BUILD_KERNEL)/core_asm.o -f elf32 $(SRC_KERNEL)/core.asm
	gcc -m32 -fno-pie -c -Isrc -o $(BUILD_KERNEL)/core_c.o $(SRC_KERNEL)/core.c
	ld --oformat binary -m elf_i386 --trace -Ttext 0x0000 --strip-all -o $(kernel_core) $(BUILD_KERNEL)/core_asm.o $(BUILD_KERNEL)/core_c.o $(BUILD_LIB_UTILS)/libutils $(BUILD_DRIVERS)/keyboard/libkeyboard $(BUILD_DRIVERS)/display/libtm_vga
	truncate --size=%512 $(kernel_core)

# Libraries

$(app_entry): $(SRC_LIB)/app/entry.asm
	mkdir -p $$(dirname $(app_entry))
	nasm -o $@ -f elf32 $<

$(BUILD_DRIVERS)/display/libtm_bios: $(SRC_DRIVERS)/display/text_mode_bios.c $(SRC_DRIVERS)/display/text_mode_bios.asm $(SRC_DRIVERS)/display/text_mode.h
	# 16 bit mode
	mkdir -p $(BUILD_DRIVERS)/display/
	gcc -m16 -fno-pie -c -Isrc -o $(BUILD_DRIVERS)/display/text_mode_bios_c.o $(SRC_DRIVERS)/display/text_mode_bios.c
	nasm -o $(SRC_DRIVERS)/display/text_mode_bios_asm.o -f elf32 $(SRC_DRIVERS)/display/text_mode_bios.asm
	ar rc 	$@ $(BUILD_DRIVERS)/display/text_mode_bios_c.o $(SRC_DRIVERS)/display/text_mode_bios_asm.o

$(BUILD_DRIVERS)/display/libtm_vga: $(SRC_DRIVERS)/display/text_mode_vga.c $(SRC_DRIVERS)/display/text_mode_vga.asm $(SRC_DRIVERS)/display/text_mode.h
	mkdir -p $(BUILD_DRIVERS)/display/
	gcc -m32 -fno-pie -c -Isrc -o $(BUILD_DRIVERS)/display/text_mode_vga_c.o $(SRC_DRIVERS)/display/text_mode_vga.c
	nasm -o $(SRC_DRIVERS)/display/text_mode_vga_asm.o -f elf32 $(SRC_DRIVERS)/display/text_mode_vga.asm
	ar rc $@ $(BUILD_DRIVERS)/display/text_mode_vga_c.o $(SRC_DRIVERS)/display/text_mode_vga_asm.o

$(BUILD_DRIVERS)/keyboard/libkeyboard: $(SRC_DRIVERS)/keyboard/keyboard.cpp $(SRC_DRIVERS)/keyboard/keyboard.asm $(SRC_DRIVERS)/keyboard/keyboard.h $(SRC_LIB_UTILS)/time.h
	mkdir -p $(BUILD_DRIVERS)/keyboard/
	g++ -m32 -fno-pie -c -Isrc -o $(BUILD_DRIVERS)/keyboard/keyboard_c.o $(SRC_DRIVERS)/keyboard/keyboard.cpp
	nasm -o $(SRC_DRIVERS)/keyboard/keyboard_asm.o -f elf32 $(SRC_DRIVERS)/keyboard/keyboard.asm
	ar rc $@ $(BUILD_DRIVERS)/keyboard/keyboard_c.o $(SRC_DRIVERS)/keyboard/keyboard_asm.o

$(BUILD_LIB_UTILS)/libutils_16: $(SRC_LIB_UTILS)/io.c $(SRC_LIB_UTILS)/io.h $(SRC_LIB_UTILS)/string.c $(SRC_LIB_UTILS)/string.h $(SRC_LIB_UTILS)/disk.c $(SRC_LIB_UTILS)/disk.asm $(SRC_LIB_UTILS)/disk.h  $(SRC_LIB_UTILS)/panic.c $(SRC_LIB_UTILS)/panic.h $(SRC_LIB_UTILS)/panic.asm $(SRC_LIB_UTILS)/time.c $(SRC_LIB_UTILS)/time.h $(SRC_LIB_UTILS)/time.asm $(SRC_LIB_UTILS)/color.c $(SRC_LIB_UTILS)/color.h
	mkdir -p $(BUILD_LIB_UTILS)/
	gcc -m16 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/io.o $(SRC_LIB_UTILS)/io.c
	gcc -m16 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/string.o $(SRC_LIB_UTILS)/string.c
	gcc -m16 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/color.o $(SRC_LIB_UTILS)/color.c
	gcc -m16 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/disk_c.o $(SRC_LIB_UTILS)/disk.c
	nasm -o $(BUILD_LIB_UTILS)/disk_asm.o -f elf32 $(SRC_LIB_UTILS)/disk.asm
	gcc -m16 -fno-pie -c -D__SOURCE_SNAPSHOT__=$(SOURCE_SNAPSHOT) -Isrc -o $(BUILD_LIB_UTILS)/panic_c.o $(SRC_LIB_UTILS)/panic.c
	nasm -o $(BUILD_LIB_UTILS)/panic_asm.o -f elf32 $(SRC_LIB_UTILS)/panic.asm
	gcc -m16 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/time_c.o $(SRC_LIB_UTILS)/time.c
	nasm -o $(BUILD_LIB_UTILS)/time_asm.o -f elf32 $(SRC_LIB_UTILS)/time.asm
	ar rc $@ $(BUILD_LIB_UTILS)/io.o $(BUILD_LIB_UTILS)/string.o $(BUILD_LIB_UTILS)/color.o $(BUILD_LIB_UTILS)/disk_c.o $(BUILD_LIB_UTILS)/disk_asm.o $(BUILD_LIB_UTILS)/panic_c.o $(BUILD_LIB_UTILS)/panic_asm.o $(BUILD_LIB_UTILS)/time_c.o $(BUILD_LIB_UTILS)/time_asm.o

$(BUILD_LIB_UTILS)/libutils: $(SRC_LIB_UTILS)/io.c $(SRC_LIB_UTILS)/io.h $(SRC_LIB_UTILS)/string.c $(SRC_LIB_UTILS)/string.h $(SRC_LIB_UTILS)/disk.c $(SRC_LIB_UTILS)/disk.asm $(SRC_LIB_UTILS)/disk.h  $(SRC_LIB_UTILS)/panic.c $(SRC_LIB_UTILS)/panic.h $(SRC_LIB_UTILS)/panic.asm $(SRC_LIB_UTILS)/time.c $(SRC_LIB_UTILS)/time.h $(SRC_LIB_UTILS)/time.asm $(SRC_LIB_UTILS)/color.c $(SRC_LIB_UTILS)/color.h
	mkdir -p $(BUILD_LIB_UTILS)/
	gcc -m32 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/io.o $(SRC_LIB_UTILS)/io.c
	gcc -m32 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/string.o $(SRC_LIB_UTILS)/string.c
	gcc -m32 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/color.o $(SRC_LIB_UTILS)/color.c
	gcc -m32 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/disk_c.o $(SRC_LIB_UTILS)/disk.c
	nasm -o $(BUILD_LIB_UTILS)/disk_asm.o -f elf32 $(SRC_LIB_UTILS)/disk.asm
	gcc -m32 -fno-pie -c -D__SOURCE_SNAPSHOT__=$(SOURCE_SNAPSHOT) -Isrc -o $(BUILD_LIB_UTILS)/panic_c.o $(SRC_LIB_UTILS)/panic.c
	nasm -o $(BUILD_LIB_UTILS)/panic_asm.o -f elf32 $(SRC_LIB_UTILS)/panic.asm
	gcc -m32 -fno-pie -c -Isrc -o $(BUILD_LIB_UTILS)/time_c.o $(SRC_LIB_UTILS)/time.c
	nasm -o $(BUILD_LIB_UTILS)/time_asm.o -f elf32 $(SRC_LIB_UTILS)/time.asm
	ar rc $@ $(BUILD_LIB_UTILS)/io.o $(BUILD_LIB_UTILS)/string.o $(BUILD_LIB_UTILS)/color.o $(BUILD_LIB_UTILS)/disk_c.o $(BUILD_LIB_UTILS)/disk_asm.o $(BUILD_LIB_UTILS)/panic_c.o $(BUILD_LIB_UTILS)/panic_asm.o $(BUILD_LIB_UTILS)/time_c.o $(BUILD_LIB_UTILS)/time_asm.o

# User Applications
$(app_calc): $(app_entry) $(SRC_APP)/calc.c $(SRC_LIB_UTILS)/io.h $(SRC_LIB_UTILS)/time.h $(BUILD_LIB_UTILS)/libutils $(BUILD_DRIVERS)/display/libtm_vga  # And dependecies :/
	mkdir -p $$(dirname $(app_calc))
	gcc -m16 -fno-pie -c -Isrc -o $(BUILD_APP)/calc.o $(SRC_APP)/calc.c
	ld --oformat binary -m elf_i386 -Ttext 0x2000 --strip-all -o $@ $(app_entry) $(BUILD_APP)/calc.o $(BUILD_LIB_UTILS)/libutils $(BUILD_DRIVERS)/display/libtm_vga
	truncate --size=%512 $@

$(app_tick_tac_toe): $(app_entry) $(SRC_APP)/tic_tac_toe.c $(SRC_LIB_UTILS)/io.h $(SRC_LIB_UTILS)/time.h $(BUILD_LIB_UTILS)/libutils $(BUILD_DRIVERS)/display/libtm_vga # And dependecies :/
	mkdir -p $$(dirname $(app_tick_tac_toe))
	gcc -m16 -fno-pie -c -Isrc -o $(BUILD_APP)/tic_tac_toe.o $(SRC_APP)/tic_tac_toe.c
	ld --oformat binary -m elf_i386 -Ttext 0x2000 --strip-all -o $@ $(app_entry) $(BUILD_APP)/tic_tac_toe.o $(BUILD_LIB_UTILS)/libutils $(BUILD_DRIVERS)/display/libtm_vga
	truncate --size=%512 $@