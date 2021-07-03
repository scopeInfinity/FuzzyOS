SRC_USR_LIB=$(SRC_DIR)/usr/lib
BUILD_USR_LIB=$(BUILD_DIR)/usr/lib

$(BUILD_USR_LIB)/%.o: $(SRC_USR_LIB)/%.c $(BUILD_USR_INCLUDE_ALL) $(SRC_LIB_UTILS)/output.h $(SRC_LIB_UTILS)/input.h $(SRC_LIB)/app/entry.h
	mkdir -p $(BUILD_USR_LIB)
	$(KERNEL_CC) -c -o $@ $<

$(BUILD_USR_LIB)/libfuzzyc:	$(patsubst $(SRC_USR_LIB)/%.c,$(BUILD_USR_LIB)/%.o,$(wildcard $(SRC_USR_LIB)/*.c))
	ar rc $@ $^
