$(BUILD_LIB_SYSCALL)/libsyscall: $(SRC_LIB_SYSCALL)/syscall.h $(SRC_LIB_SYSCALL)/syscall.asm
	mkdir -p $(BUILD_LIB_SYSCALL)/
	nasm -o $(BUILD_LIB_SYSCALL)/syscall_asm.o -f elf32 $(SRC_LIB_SYSCALL)/syscall.asm
	ar rc $@ $(BUILD_LIB_SYSCALL)/syscall_asm.o
