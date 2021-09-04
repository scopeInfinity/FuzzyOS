#include <fuzzy/kernel/syscall/console.h>

#include <stdio.h>

#include <lib/utils/output.h>

#define CONSOLE_PUTS_BUFFER_SIZE 128

static int _console_putchar(char c) {
    print_char(c);
    return 0;
}

static int _console_puts_buffer(int user_ds, const char *__us_str, int len) {
    char str[CONSOLE_PUTS_BUFFER_SIZE]={0};
    while (len > 0) {
        int sub_len = min(CONSOLE_PUTS_BUFFER_SIZE, len);
        syscall_strncpy_user_to_kernel(user_ds, __us_str, str, sub_len);

        for(int i=0;i<sub_len;i++) {
            print_char(str[i]);
        }

        len -= sub_len;
        __us_str += sub_len;
    }
    return 0;
}

static int _console_clrscr() {
    print_rectangle(0, 0, TEXT_WINDOW_WIDTH-1, TEXT_WINDOW_HEIGHT-1);
    move_xy(0,0);
}

int syscall_3_console(int operation, int a1, int a2, int a3, int user_ds) {
    switch (operation) {
        case SYSCALL_CONSOLE_SUB_CLRSCR:
            return _console_clrscr();
        case SYSCALL_CONSOLE_SUB_PUTCHAR:
            return _console_putchar((char)a1);
        case SYSCALL_CONSOLE_SUB_PUTS_BUFFER:
            return _console_puts_buffer(user_ds, (const char*)a1, a2);
    }
    return -1;
}

#undef CONSOLE_PUTS_BUFFER_SIZE
