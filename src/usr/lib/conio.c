// #include <lib/syscall/syscall.h>
#include <lib/utils/output.h>
#include <lib/utils/input.h>
#include <lib/app/entry.h>

char getch() {
    // return '0';
    return getch_in();
}

void gotoxy(unsigned char x, unsigned char y) {
    move_xy(x, y);
}

void clrscr() {
    entry_console_init();
}