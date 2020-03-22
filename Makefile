BL_SRC_DIR = src/bootloader
BUILD_DIR = build

.PHONY: all clean

bt_stage1 = $(BUILD_DIR)/bt_stage1
image_vmdk = $(BUILD_DIR)/image.vmdk

all: images binaries

images: $(image_vmdk)

binaries: $(bt_stage1)

$(image_vmdk): $(bt_stage1)
	dd bs=512 if=$< of=$@
	/bin/echo -ne "\x55\xaa" | dd seek=510 bs=1 of=$@

$(bt_stage1): $(BL_SRC_DIR)/stage1.asm $(BL_SRC_DIR)/constants.asm $(BL_SRC_DIR)/io.asm
	nasm -o $@ -f bin -i $(BL_SRC_DIR)/ $<

clean:
	rm -f $(image_vmdk) $(bt_stage1)
