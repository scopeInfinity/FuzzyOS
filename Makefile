BL_SRC_DIR = src/bootloader
SYSCALLS_SRC_DIR = src/lib/syscalls
LIB_SRC_DIR = src/lib
UTIL_SRC_DIR = src/lib/util
BUILD_DIR = build
APP_DIR = src/app

.PHONY: all clean

# Files
bt_stage1 = $(BUILD_DIR)/bt_stage1
bt_stage2_c_o = $(BUILD_DIR)/bt_stage2_c.o
bt_stage2_asm_o = $(BUILD_DIR)/bt_stage2_asm.o
bt_stage2 = $(BUILD_DIR)/bt_stage2
image_vmdk = $(BUILD_DIR)/image.vmdk
app_entry_o = $(BUILD_DIR)/app_entry.o

# Apps
app_calc = $(BUILD_DIR)/calc
app_ttt = $(BUILD_DIR)/ttt
app_dashboard = $(BUILD_DIR)/dashboard

# Parameters
BT_STAGE2_SECTOR_COUNT = 19 # In Hex

rebuild: clean all

all: images binaries

images: $(image_vmdk)

binaries: $(bt_stage1) $(bt_stage2)

$(image_vmdk): $(bt_stage1) $(bt_stage2)  $(app_calc) $(app_ttt)
	dd bs=512 count=2 if=$(bt_stage1) of=$@
	/bin/echo -ne "\x55\xaa" | dd seek=510 bs=1 of=$@
	cat $(bt_stage2) >> $@
	cat $(app_calc)  >> $@
	cat $(app_ttt)  >> $@
	@echo "Stage 1 Size     : " $$(stat -c %s $(bt_stage1))
	@echo "Stage 2 Size     : " $$(stat -c %s $(bt_stage2))
	@echo "App Calc Size    : " $$(stat -c %s $(app_calc))
	@echo "App TTT Size    : " $$(stat -c %s $(app_ttt))
	@echo "Image Size       : " $$(stat -c %s $@)
	@echo "Want BT_STAGE2_SECTOR_COUNT : 0x"$$(printf "%x\n" $$(( $$(stat -c %s $(bt_stage2)) / 512)) )
	@echo "Got BT_STAGE2_SECTOR_COUNT  : 0x"$(BT_STAGE2_SECTOR_COUNT)
	@echo "AppCalc Sector Count : "$$(( $$(stat -c %s $(app_calc)) / 512))
	@echo "AppCalc Sector Start : "$$(( 1 + $$(stat -c %s $(bt_stage1)) / 512 + $$(stat -c %s $(bt_stage2)) / 512))
	@echo "AppTTT Sector Count : "$$(( $$(stat -c %s $(app_ttt)) / 512))
	@echo "AppTTT Sector Start : "$$(( 1 + $$(stat -c %s $(bt_stage1)) / 512 + $$(stat -c %s $(bt_stage2)) / 512 + $$(stat -c %s $(app_calc)) / 512))

$(bt_stage1): $(BL_SRC_DIR)/stage1.asm $(BL_SRC_DIR)/constants.asm $(BL_SRC_DIR)/io.asm $(BL_SRC_DIR)/disk.asm
	nasm -o $@ -f bin -i $(BL_SRC_DIR)/ -D BT_STAGE2_SECTOR_COUNT=$(BT_STAGE2_SECTOR_COUNT) $<
	truncate --size=%512 $@

$(bt_stage2): $(bt_stage2_asm_o) $(bt_stage2_c_o)
	ld --oformat binary -m elf_i386 -Ttext 0x8000 --strip-all -o $@ $^
	truncate --size=%512 $@

$(bt_stage2_c_o): $(BL_SRC_DIR)/stage2.c $(SYSCALLS_SRC_DIR)/basic.h $(SYSCALLS_SRC_DIR)/io.h $(SYSCALLS_SRC_DIR)/time.h $(SYSCALLS_SRC_DIR)/color.h $(UTIL_SRC_DIR)/string.h $(APP_DIR)/dashboard.c
	gcc -m16 -fno-pie -c -Isrc -o $@ $<

$(bt_stage2_asm_o): $(BL_SRC_DIR)/stage2.asm $(BL_SRC_DIR)/constants.asm $(BL_SRC_DIR)/io.asm $(SYSCALLS_SRC_DIR)/io_syscall.asm $(SYSCALLS_SRC_DIR)/time_syscall.asm $(bt_stage2_o) $(SYSCALLS_SRC_DIR)/disk_syscall.asm
	nasm -o $@ -f elf32 -i $(BL_SRC_DIR)/ -i$(SYSCALLS_SRC_DIR)/ $<

$(app_entry_o): $(LIB_SRC_DIR)/app/entry.asm $(BL_SRC_DIR)/constants.asm $(BL_SRC_DIR)/io.asm $(SYSCALLS_SRC_DIR)/io_syscall.asm $(SYSCALLS_SRC_DIR)/time_syscall.asm
	nasm -o $@ -f elf32 -i $(BL_SRC_DIR)/ -i$(SYSCALLS_SRC_DIR)/ $<

$(app_calc): $(app_entry_o) $(APP_DIR)/calc.c $(SYSCALLS_SRC_DIR)/basic.h $(SYSCALLS_SRC_DIR)/io.h $(SYSCALLS_SRC_DIR)/time.h $(SYSCALLS_SRC_DIR)/color.h $(UTIL_SRC_DIR)/string.h $(BL_SRC_DIR)/constants.asm $(BL_SRC_DIR)/io.asm $(SYSCALLS_SRC_DIR)/io_syscall.asm $(SYSCALLS_SRC_DIR)/time_syscall.asm
	gcc -m16 -fno-pie -c -Isrc -o $(BUILD_DIR)/calc.o $(APP_DIR)/calc.c
	ld --oformat binary -m elf_i386 -Ttext 0xC000 --strip-all -o $@ $(app_entry_o) $(BUILD_DIR)/calc.o
	truncate --size=%512 $@

$(app_ttt): $(app_entry_o) $(APP_DIR)/tic_tac_toe.c $(SYSCALLS_SRC_DIR)/basic.h $(SYSCALLS_SRC_DIR)/io.h $(SYSCALLS_SRC_DIR)/time.h $(SYSCALLS_SRC_DIR)/color.h $(UTIL_SRC_DIR)/string.h $(BL_SRC_DIR)/constants.asm $(BL_SRC_DIR)/io.asm $(SYSCALLS_SRC_DIR)/io_syscall.asm $(SYSCALLS_SRC_DIR)/time_syscall.asm
	gcc -m16 -fno-pie -c -Isrc -o $(BUILD_DIR)/ttt.o $(APP_DIR)/tic_tac_toe.c
	ld --oformat binary -m elf_i386 -Ttext 0xC000 --strip-all -o $@ $(app_entry_o) $(BUILD_DIR)/ttt.o
	truncate --size=%512 $@

debug_stage1: $(bt_stage1)
	objdump -b binary -mi386 -Maddr16,data16 -D $<
	xxd $<

debug_stage2: $(bt_stage2)
	objdump -b binary -mi386 -Maddr16,data16 -D $<
	xxd $<

debug_stage2_c: $(bt_stage2_c_o)
	objdump -d -Maddr16,data16 $<
	xxd $<

debug_stage2_asm: $(bt_stage2_asm_o)
	objdump -d -Maddr16,data16 $<
	xxd $<

qemu: $(image_vmdk) images
	cpulimit -f -l 10 -- qemu-system-x86_64 -smp 1 -m 1M -hda $<

clean:
	rm -f $(image_vmdk) $(bt_stage1) $(bt_stage2) $(bt_stage2_c_o) $(bt_stage2_asm_o)
