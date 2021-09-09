#include <fuzzy/kernel/interrupts/interrupts.h>
#include <fuzzy/kernel/process/process.h>
#include <fuzzy/kernel/syscall/file_handler.h>
#include <fuzzy/kernel/syscall/console.h>
#include <fuzzy/kernel/syscall/graphics.h>
#include <fuzzy/kernel/syscall/keyboard.h>
#include <fuzzy/kernel/syscall/syscall.h>

#include <sys/syscall.h>

#include <drivers/keyboard/keyboard.h>
#include <lib/utils/logging.h>

#define SYSCALL_SIZE 30

int SYSCALL_TABLE[SYSCALL_SIZE];

extern int interrupt_handler_0x32_syscall_handler();

void interrupt_register_0x32_syscall() {
    print_log("Registering syscalls.");
    populate_idt_entry_32bit(IDT_SYSCALL, (unsigned int)interrupt_handler_0x32_syscall_handler, 0, 1);
    SYSCALL_TABLE[SYSCALL_KEYBOARD]=syscall_0_keyboard;
    SYSCALL_TABLE[SYSCALL_PROCESS]=syscall_1_process;
    SYSCALL_TABLE[SYSCALL_FILE_OP]=syscall_2_file_handler;
    SYSCALL_TABLE[SYSCALL_CONSOLE]=syscall_3_console;
    SYSCALL_TABLE[SYSCALL_GRAPHICS]=syscall_4_graphics;
}

int syscall_selector(int id, int arg0, int arg1, int arg2, int arg3, int user_ds) {
    return ((int(*)(int,int,int,int,int))(SYSCALL_TABLE[id]))(arg0, arg1, arg2, arg3, user_ds);
}
