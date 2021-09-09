#include <sys/syscall.h>
#include <stdio.h>
#include <conio.h>

int getch() {
    // syscall is irq blocking,
    // so try to keep them as short as possible.
    while (1) {
        char c = SYSCALL_A1(SYSCALL_KEYBOARD, SYSCALL_KEYBOARD_SUB_GETCH);
        if (c) return c;
        yield();
    }
}

int kbhit() {
    return SYSCALL_A1(SYSCALL_KEYBOARD, SYSCALL_KEYBOARD_SUB_KBHIT);
}

void clrscr() {
    SYSCALL_A1(SYSCALL_CONSOLE, SYSCALL_CONSOLE_SUB_CLRSCR);
}
