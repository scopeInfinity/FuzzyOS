#include <sys/syscall.h>
#include <stdio.h>

char getch() {
    return SYSCALL_A0(SYSCALL_KEYBOARD);
}

void clrscr() {
    SYSCALL_A1(SYSCALL_CONSOLE, SYSCALL_CONSOLE_SUB_CLRSCR);
}