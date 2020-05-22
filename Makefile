BL_SRC_DIR = src/bootloader
BUILD_DIR = build

.PHONY: all clean

bt_stage1 = $(BUILD_DIR)/bt_stage1
bt_stage2_o = $(BUILD_DIR)/bt_stage2.o
bt_stage2_asm_o = $(BUILD_DIR)/bt_stage2_asm.o
bt_stage2 = $(BUILD_DIR)/bt_stage2
image_vmdk = $(BUILD_DIR)/image.vmdk

all: images binaries

images: $(image_vmdk)

binaries: $(bt_stage1) $(bt_stage2)

$(image_vmdk): $(bt_stage1) $(bt_stage2)
	dd bs=512 count=2 if=$(bt_stage1) of=$@
	/bin/echo -ne "\x55\xaa" | dd seek=510 bs=1 of=$@
	dd seek=1 bs=512 if=$(bt_stage2) of=$@
	dd seek=2 bs=512 count=1 if=/dev/zero of=$@

$(bt_stage1): $(BL_SRC_DIR)/stage1.asm $(BL_SRC_DIR)/constants.asm $(BL_SRC_DIR)/io.asm $(BL_SRC_DIR)/disk.asm
	nasm -o $@ -f bin -i $(BL_SRC_DIR)/ $<

# Compile in 32-bit mode
$(bt_stage2_o): $(BL_SRC_DIR)/stage2.c
	gcc -c -o $@ $<

# $(bt_stage2_asm_o): $(BL_SRC_DIR)/stage2.asm
# 	gcc -c -o $@ $<

$(bt_stage2): $(BL_SRC_DIR)/stage2.asm $(BL_SRC_DIR)/constants.asm $(BL_SRC_DIR)/io.asm $(bt_stage2_o)
	nasm -o $@ -f bin -i $(BL_SRC_DIR)/ $<

clean:
	rm -f $(image_vmdk) $(bt_stage1) $(bt_stage2) $(bt_stage2_o) $(bt_stage2_asm_o)
