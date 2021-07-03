$(BUILD_LIB_SYSCALL)/libsyscall: $(SRC_LIB_SYSCALL)/syscall.h $(SRC_LIB_SYSCALL)/syscall.asm
	mkdir -p $(BUILD_LIB_SYSCALL)/
	nasm -o $(BUILD_LIB_SYSCALL)/syscall_asm.o -f elf32 $(SRC_LIB_SYSCALL)/syscall.asm
	ar rc $@ $(BUILD_LIB_SYSCALL)/syscall_asm.o


# BUILD_LIB_SYSCALL_ALL = $(addprefix $(BUILD_LIB_SYSCALL)/,syscall_asm.o)

# $(BUILD_LIB_SYSCALL)/%_asm.o: $(SRC_LIB_SYSCALL)/%.asm
# 	mkdir -p $(BUILD_LIB)/app
# 	nasm -o $@ -f elf32 $<

# $(BUILD_LIB_SYSCALL)/%.o: $(SRC_LIB_SYSCALL)/%.c
# 	mkdir -p $(BUILD_LIB_SYSCALL)
# 	$(KERNEL_CC) -c -o $@ $<
