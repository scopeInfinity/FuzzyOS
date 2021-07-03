SRC_USR_LIB=$(SRC_DIR)/usr/lib
BUILD_USR_LIB=$(BUILD_DIR)/usr/lib

KERNEL_CC = $(CC) -m32 -fno-pie --sysroot=$(BUILD_DIR)

$(BUILD_USR_LIB)/%.o: $(SRC_USR_LIB)/%.c $(BUILD_USR_INCLUDE_ALL)
	mkdir -p $(BUILD_USR_LIB)
	$(KERNEL_CC) -c -o $@ $<

$(BUILD_USR_LIB)/libfuzzyc:	$(patsubst $(SRC_USR_LIB)/%.c,$(BUILD_USR_LIB)/%.o,$(wildcard $(SRC_USR_LIB)/*.c))
	ar rc $@ $^
