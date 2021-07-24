#include <lib/utils/logging.h>
#include <drivers/keyboard/keyboard.h>

#include "kernel/extern.h"
#include "kernel/process.c"
#include "kernel/syscall/file_handler.h"

#define SYSCALL_SIZE 30

int SYSCALL_TABLE[SYSCALL_SIZE];


int syscall_0_keyboard_getch(int a0,int a1,int a2,int a3) {
    syscall_context_switching_fix_es();
    return keyboard_get_key_pressed_blocking();
}

int syscall_1_process_exec(int sector_start, int sector_count, int a2,int a3) {
    syscall_context_switching_fix_es();
    return process_exec(sector_start, sector_count);
}


void register_syscalls() {
    print_log("Registering syscalls.");
    SYSCALL_TABLE[0]=syscall_0_keyboard_getch;
    SYSCALL_TABLE[1]=syscall_1_process_exec;
    SYSCALL_TABLE[2]=syscall_2_file_handler;
}