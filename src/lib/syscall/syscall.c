#include<lib/syscall/syscall.h>

int syscall_keyboard() {
    return SYSCALL_A0(SYSCALL_KEYBOARD);
}

int syscall_process(int sector_start,int sector_count) {
    return SYSCALL_A2(SYSCALL_PROCESS, sector_start, sector_count);
}
