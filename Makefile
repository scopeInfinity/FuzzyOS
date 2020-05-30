BL_SRC_DIR = src/bootloader
BUILD_DIR = build

.PHONY: all clean

# Files
bt_stage1 = $(BUILD_DIR)/bt_stage1
bt_stage2_c_o = $(BUILD_DIR)/bt_stage2_c.o
bt_stage2_asm_o = $(BUILD_DIR)/bt_stage2_asm.o
bt_stage2 = $(BUILD_DIR)/bt_stage2
image_vmdk = $(BUILD_DIR)/image.vmdk

# Parameters
BT_STAGE2_SECTOR_COUNT = 11 # In Hex

rebuild: clean all

all: images binaries

images: $(image_vmdk)

binaries: $(bt_stage1) $(bt_stage2)

$(image_vmdk): $(bt_stage1) $(bt_stage2)
	dd bs=512 count=2 if=$(bt_stage1) of=$@
	/bin/echo -ne "\x55\xaa" | dd seek=510 bs=1 of=$@
	dd seek=1 bs=512 if=$(bt_stage2) of=$@
	truncate --size=%512 $@
	@echo "Stage 1 Size     : " $$(stat -c %s $(bt_stage1))
	@echo "Stage 2 Size     : " $$(stat -c %s $(bt_stage2))
	@echo "Stage 2 LoadSize : " $$(( $$(printf "%d\n" "0x"$(BT_STAGE2_SECTOR_COUNT)) * 512 ))
	@echo "Image Size       : " $$(stat -c %s $@)
	@echo "Want BT_STAGE2_SECTOR_COUNT : 0x"$$(printf "%x\n" $$(( $$(stat -c %s $@) / 512 - 1)) )
	@echo "Got BT_STAGE2_SECTOR_COUNT  : 0x"$(BT_STAGE2_SECTOR_COUNT)

$(bt_stage1): $(BL_SRC_DIR)/stage1.asm $(BL_SRC_DIR)/constants.asm $(BL_SRC_DIR)/io.asm $(BL_SRC_DIR)/disk.asm
	nasm -o $@ -f bin -i $(BL_SRC_DIR)/ -D BT_STAGE2_SECTOR_COUNT=$(BT_STAGE2_SECTOR_COUNT) $<

$(bt_stage2): $(bt_stage2_asm_o) $(bt_stage2_c_o)
	ld --oformat binary -m elf_i386 -Ttext 0x8000 --strip-all -o $@ $^

$(bt_stage2_c_o): $(BL_SRC_DIR)/stage2.c
	gcc -m16 -fno-pie -c -o $@ $<

$(bt_stage2_asm_o): $(BL_SRC_DIR)/stage2.asm $(BL_SRC_DIR)/constants.asm $(BL_SRC_DIR)/io.asm $(bt_stage2_o)
	nasm -o $@ -f elf32 -i $(BL_SRC_DIR)/ $<

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

clean:
	rm -f $(image_vmdk) $(bt_stage1) $(bt_stage2) $(bt_stage2_c_o) $(bt_stage2_asm_o)
