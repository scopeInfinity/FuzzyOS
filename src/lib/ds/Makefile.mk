$(BUILD_LIB_DS)/libds: $(SRC_LIB_DS)/queue.h $(SRC_LIB_DS)/queue.c
	mkdir -p $(BUILD_LIB_DS)/
	$(CC) -m32 -fno-pie -c -Isrc -o $(BUILD_LIB_DS)/queue.o $(SRC_LIB_DS)/queue.c
	ar rc $@ $(BUILD_LIB_DS)/queue.o