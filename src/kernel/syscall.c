#include <lib/utils/logging.h>
#include <drivers/keyboard/keyboard.h>

#include "kernel/extern.h"
#include "kernel/process.c"
#include "kernel/syscall/file_handler.h"

#define SYSCALL_SIZE 30

int SYSCALL_TABLE[SYSCALL_SIZE];


int syscall_0_keyboard_getch(int a0,int a1,int a2,int a3, int user_ds) {
    return keyboard_get_key_pressed_blocking();
}

void register_syscalls() {
    print_log("Registering syscalls.");
    SYSCALL_TABLE[0]=syscall_0_keyboard_getch;
    SYSCALL_TABLE[1]=syscall_1_process;
    SYSCALL_TABLE[2]=syscall_2_file_handler;
}