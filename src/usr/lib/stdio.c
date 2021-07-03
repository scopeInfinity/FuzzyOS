#include <stdio.h>
#include <lib/utils/output.h>

int puts (const char *s) {
    print_line(s);
    // TODO: Fix puts return value.
    return 0;
}

int putchar(int c) {
    print_char(c);
    // TODO: Fix putchar return value.
    return 0;
}
