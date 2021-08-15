#include <fuzzy/kernel/syscall/console.h>

#include <stdio.h>

#include <lib/utils/output.h>

int _console_putchar(char c) {
    print_char(c);
    return 0;
}

int _console_clrscr() {
    print_rectangle(0, 0, TEXT_WINDOW_WIDTH-1, TEXT_WINDOW_HEIGHT-1);
    move_xy(0,0);
}

int syscall_3_console(int operation, int a1, int a2, int a3, int user_ds) {
    switch (operation) {
        case SYSCALL_CONSOLE_SUB_CLRSCR:
            return _console_clrscr();
        case SYSCALL_CONSOLE_SUB_PUTCHAR:
            return _console_putchar((char)a1);
    }
    return -1;
}

