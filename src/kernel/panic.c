#include <fuzzy/kernel/panic.h>

extern void panic_just_halt();

int panic(int err, const char *message, const char *src_file,
    unsigned int line_number) {
    set_color_bg(C_WHITE);
    set_color_fg(C_RED);
    move_xy(0,0);
    print_line(src_file);
    print_char(':');
    print_int(line_number);
    print_char(',');
    print_line(__SOURCE_SNAPSHOT__);
    move_xy(0,1);
    print_line("Panic");
    if (err>0) {
        print_char('[');
        print_int(err);
        print_char(']');
    }
    print_line(": ");
    print_line(message);
    panic_just_halt();
}
