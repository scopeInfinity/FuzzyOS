#include <lib/utils/logging.h>
#include <drivers/keyboard/keyboard.h>

#include "kernel/process.c"

#define SYSCALL_SIZE 30
int SYSCALL_TABLE[SYSCALL_SIZE];

int syscall_0_keyboard_getch(int a0,int a1,int a2,int a3) {
    return keyboard_get_key_pressed_blocking();
}

int syscall_1_process_exec(int sector_start,int sector_count,int a2,int a3) {
    return process_exec(sector_start, sector_count);
}

int syscall_2_process_status(int id,int a1,int a2,int a3) {
    struct Process *process = process_get(id);
    return process->state;
}

int syscall_3_process_exitcode(int id,int a1,int a2,int a3) {
    struct Process *process = process_get(id);
    return process->exit_code;
}

void register_syscalls() {
    print_log("Registering syscalls.");
    SYSCALL_TABLE[0]=syscall_0_keyboard_getch;
    SYSCALL_TABLE[1]=syscall_1_process_exec;
    SYSCALL_TABLE[2]=syscall_2_process_status;
    SYSCALL_TABLE[3]=syscall_3_process_exitcode;
}