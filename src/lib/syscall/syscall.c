#include<lib/syscall/syscall.h>

int syscall_keyboard() {
    return SYSCALL_A0(SYSCALL_KEYBOARD);
}

int syscall_process(int sector_start,int sector_count) {
    return SYSCALL_A2(SYSCALL_PROCESS, sector_start, sector_count);
}

int syscall_process_state(int id) {
    return SYSCALL_A1(SYSCALL_PROCESS_STATE, id);
}

int syscall_process_exitcode(int id) {
    return SYSCALL_A1(SYSCALL_PROCESS_EXITCODE, id);
}

int syscall_process_fork() {
    return SYSCALL_A0(SYSCALL_PROCESS_FORK);
}