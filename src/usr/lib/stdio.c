#include <stdio.h>
#include <stddef.h>
#include <conio.h>
#include <lib/utils/output.h>

int putchar(int c) {
    print_char(c);
    // TODO: Fix putchar return value.
    return 0;
}

int puts(const char *s) {
    print_line(s);
    // TODO: Fix puts return value.
    return 0;
}

char* gets(char *s) {
    while (1) {
        char c = getch();
        if (c == '\0') return NULL;
        if (c == '\n') return s;
        putchar(c);
        (*s) = c;
        s++;
    }
}
