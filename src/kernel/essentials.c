#ifndef __KERNEL_ESSENTIALS
#define __KERNEL_ESSENTIALS

#include <lib/syscalls/io.h>

// __low_panic_halt defined in core.asm.
extern void __low_panic_halt();

#define PANIC(err, message) panic((err), (message), __FILE__, __LINE__, __SOURCE_SNAPSHOT__)

void panic(int err, const char *message, const char *src_file,
    unsigned int line_number, const char *src_snapshot) {
    set_color_bg(C_WHITE);
    set_color_fg(C_RED);
    move_xy(0,0);
    print_line(src_file);
    print_char(':');
    print_int(line_number);
    print_char(',');
    print_line(src_snapshot);
    move_xy(0,1);
    print_line("Panic");
    if (err>0) {
        print_char('[');
        print_int(err);
        print_char(']');
    }
    print_line(": ");
    print_line(message);
    __low_panic_halt();
}

extern unsigned int __low_va_args(unsigned int index);
#define va_args(type, index) ((type)(__low_va_args((index))))

void kernel_status(const char *message, int status) {
    set_color_bg(C_WHITE);
    set_color_fg(C_BLACK);
    move_xy(0, WINDOW_HEIGHT-1);
    print_line(message);
    print_int(status);
    int left = WINDOW_WIDTH - get_cursor_x();
    while(left>0) {
        print_char(' ');
        left--;
    }
}

#endif
