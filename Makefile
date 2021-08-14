ROOT_DIR = .
BUILD_DIR = build
INCLUDE_DIR = include/fuzzy
SRC_DIR = src

SELF_SRC_DIR = $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
SELF_INCLUDE_DIR = $(patsubst $(SRC_DIR)/%,$(INCLUDE_DIR)/%,$(SELF_SRC_DIR))
SELF_BUILD_DIR = $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(SELF_SRC_DIR))

SELF_SRC_ALL_C = $(shell find $(SELF_SRC_DIR) -name '*.c')
SELF_BUILD_ALL_C = $(patsubst $(SELF_SRC_DIR)/%.c,$(SELF_BUILD_DIR)/%.o,$(SELF_SRC_ALL_C))
SELF_SRC_ALL_ASM = $(shell find $(SELF_SRC_DIR) -name '*.asm')
SELF_BUILD_ALL_ASM = $(patsubst $(SELF_SRC_DIR)/%.asm,$(SELF_BUILD_DIR)/%_asm.o,$(SELF_SRC_ALL_ASM))

SRC_BOOTLOADER = $(SRC_DIR)/bootloader
SRC_KERNEL = $(SRC_DIR)/kernel
SRC_DRIVERS = $(SRC_DIR)/drivers
SRC_LIB_DS = $(SRC_DIR)/lib/ds
SRC_LIB_UTILS = $(SRC_DIR)/lib/utils
SRC_LIB = $(SRC_DIR)/lib
SRC_MEMMGR = $(SRC_DIR)/memmgr
SRC_REALMODE = $(SRC_DIR)/real_mode

BUILD_BOOTLOADER = $(BUILD_DIR)/bootloader
BUILD_KERNEL = $(BUILD_DIR)/kernel
BUILD_DRIVERS = $(BUILD_DIR)/drivers
BUILD_LIB_DS = $(BUILD_DIR)/lib/ds
BUILD_LIB_UTILS = $(BUILD_DIR)/lib/utils
BUILD_LIB = $(BUILD_DIR)/lib
BUILD_REALMODE = $(BUILD_DIR)/real_mode
.PHONY: all clean

# Files
bt_stage1 = $(BUILD_BOOTLOADER)/stage1
bt_stage2 = $(BUILD_BOOTLOADER)/stage2
image_vmdk = $(BUILD_DIR)/image.vmdk
app_entry = $(BUILD_LIB)/app/entry.out
rm_static = $(BUILD_REALMODE)/static_library

# Kernel
kernel_core = $(BUILD_DIR)/kernel/core

MINIMAL_DISK = $(BUILD_DIR)/minimal_disk


# Program to auto start when kernel is ready.
# 1 - Tic Tac Toe
# 2 - Calculator
# 3 - ls
# 4 - cat
# 5 - sh
RUN_APP_ID = 1

# Apps
SRC_APP = $(SRC_DIR)/usr/local/src
BUILD_APP = $(BUILD_DIR)/usr/local/bin
app_calc = $(BUILD_APP)/calc.out
app_tic_tac_toe = $(BUILD_APP)/tictactoe.out
app_ls = $(BUILD_APP)/ls.out
app_cat = $(BUILD_APP)/cat.out
app_sh = $(BUILD_APP)/sh.out

MEMORY_LOCATION_KERNEL = 0xC000

SOURCE_SNAPSHOT="\"$$(git rev-parse --short HEAD)$$(git diff --quiet || echo '_unstaged')\""

# General Assumptions
## Integer is 4 bytes

# Debugging controller
DEBUG?=
ifdef DEBUG
CC_DEBUG=-g
NASM_DEBUG=-g
LD_DEBUG=
else
CC_DEBUG=
NASM_DEBUG=
LD_DEBUG=--strip-all
endif

# Tools
HOST_CC = gcc -std=c11 -Iinclude

NASM=nasm -f elf32 $(NASM_DEBUG)

CC=gcc -std=c11 -fno-builtin -Os -nostartfiles -nostdlib -static $(CC_DEBUG)
KERNEL_CC = $(CC) -m32 -fno-pie -Isrc --sysroot=$(BUILD_DIR) -Iinclude -Isrc/usr/include
USER_CC = $(CC) -m32 -fno-pie -Isrc --sysroot=$(BUILD_DIR)

LD=ld -nostdlib -nostartfiles -nodefaultlibs $(LD_DEBUG)
KERNEL_LD=$(LD) -m elf_i386 -T linker.ld -Ttext 0x0
USER_LD= $(LD) -m elf_i386 -T linker.ld -Ttext 0x0

FLAT_FROM_ELF=objcopy -O binary

# Targets
all_artifacts: images binaries external

test: $(image_vmdk) $(wildcard tests/**/*)
	bash tests/run.sh

images: $(image_vmdk)

binaries: $(bt_stage1) $(bt_stage2) $(kernel_core) $(rm_static)

# Build dependecies for configure and $(image_vmdk) are ordered based on their position on disk.
# Note: Relying on default value doesn't guarantee bin size.
SECTOR_START_BT_STAGE1 = 0
SECTOR_COUNT_BT_STAGE1 = $(shell cut -d' ' -f1 configure 2> /dev/null || echo 1 )
SECTOR_START_SHARED_LIBRARY = $(shell expr $(SECTOR_START_BT_STAGE1) + $(SECTOR_COUNT_BT_STAGE1) )
SECTOR_COUNT_SHARED_LIBRARY = $(shell cut -d' ' -f2 configure 2> /dev/null || echo 1 )
SECTOR_START_BT_STAGE2 = $(shell expr $(SECTOR_START_SHARED_LIBRARY) + $(SECTOR_COUNT_SHARED_LIBRARY) )
SECTOR_COUNT_BT_STAGE2 = $(shell cut -d' ' -f3 configure 2> /dev/null || echo 30 )
SECTOR_START_KERNEL = $(shell expr $(SECTOR_START_BT_STAGE2) + $(SECTOR_COUNT_BT_STAGE2) )
SECTOR_COUNT_KERNEL = $(shell cut -d' ' -f4 configure 2> /dev/null || echo 30 )

# configure file stores the sector size of each sub images.
configure: $(bt_stage1) $(rm_static) $(bt_stage2) $(kernel_core)
	bash scripts/build_image.sh /dev/null $^ > $@
	rm -r $(BUILD_DIR)/ && "Cleared build directory" || echo "Build directory is clean."

$(image_vmdk): $(bt_stage1) $(rm_static) $(bt_stage2) $(kernel_core) $(BUILD_DIR)/external/bin/mbr_builder $(MINIMAL_DISK)
	test -s configure || { echo -e "\033[0;31mFailed! Please execute 'make configure' first.\033[0m" >&2; exit 1; }
	bash scripts/build_image.sh $(BUILD_DIR)/temp_vmdk $(bt_stage1) $(rm_static) $(bt_stage2) $(kernel_core)
	./$(BUILD_DIR)/external/bin/mbr_builder $@  $(BUILD_DIR)/temp_vmdk $(MINIMAL_DISK)
	@echo "Image Size : $$(stat -c %s $@) byte(s)"

clean:
	rm -r $(BUILD_DIR)/ || echo "Build directory is clean."
	rm -f configure

include emulator/Makefile.mk

include $(SRC_DIR)/usr/include/Makefile.mk
include $(SRC_DIR)/usr/lib/Makefile.mk

include $(SRC_BOOTLOADER)/Makefile.mk
include $(SRC_REALMODE)/Makefile.mk

include $(SRC_KERNEL)/Makefile.mk
include $(SRC_KERNEL)/interrupts/Makefile.mk
include $(SRC_KERNEL)/process/Makefile.mk
include $(SRC_KERNEL)/syscall/Makefile.mk

include $(SRC_DRIVERS)/disk/Makefile.mk
include $(SRC_DRIVERS)/display/Makefile.mk
include $(SRC_DRIVERS)/keyboard/Makefile.mk
include $(SRC_DRIVERS)/pic/Makefile.mk

include $(SRC_DIR)/fs/Makefile.mk
include $(SRC_DIR)/memmgr/tables/Makefile.mk

include $(SRC_LIB)/app/Makefile.mk
include $(SRC_LIB_DS)/Makefile.mk
include $(SRC_LIB_UTILS)/Makefile.mk

include $(SRC_APP)/Makefile.mk

include external/src/Makefile.mk
