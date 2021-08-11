#include <fuzzy/kernel/interrupts/interrupts.h>
#include <fuzzy/kernel/process/process.h>
#include <fuzzy/kernel/syscall/file_handler.h>
#include <fuzzy/kernel/syscall/syscall.h>

#include <drivers/keyboard/keyboard.h>
#include <lib/utils/logging.h>

#define SYSCALL_SIZE 30

int SYSCALL_TABLE[SYSCALL_SIZE];

int syscall_0_keyboard_getch(int a0,int a1,int a2,int a3, int user_ds) {
    return keyboard_get_key_pressed_blocking();
}

extern int interrupt_handler_0x32_syscall_handler();

void interrupt_register_0x32_syscall() {
    print_log("Registering syscalls.");
    populate_idt_entry_32bit(IDT_SYSCALL, (unsigned int)interrupt_handler_0x32_syscall_handler, 0, 1);
    SYSCALL_TABLE[0]=syscall_0_keyboard_getch;
    SYSCALL_TABLE[1]=syscall_1_process;
    SYSCALL_TABLE[2]=syscall_2_file_handler;
}

int syscall_selector(int id, int arg0, int arg1, int arg2, int arg3, int user_ds) {
    return ((int(*)(int,int,int,int,int))(SYSCALL_TABLE[id]))(arg0, arg1, arg2, arg3, user_ds);
}
