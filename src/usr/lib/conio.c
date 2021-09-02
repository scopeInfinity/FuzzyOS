#include <sys/syscall.h>
#include <stdio.h>

char getch() {
    // syscall is irq blocking,
    // so try to keep them as short as possible.
    while (1) {
        char c = SYSCALL_A0(SYSCALL_KEYBOARD);
        if (c) return c;
        yield();
    }
}

void clrscr() {
    SYSCALL_A1(SYSCALL_CONSOLE, SYSCALL_CONSOLE_SUB_CLRSCR);
}