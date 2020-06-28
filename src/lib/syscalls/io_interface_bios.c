#ifndef __LIB_SYSCALLS_IO_INTERFACE_BIOS
#define __LIB_SYSCALLS_IO_INTERFACE_BIOS
#include <lib/syscalls/io_interface.h>

extern void _low_print(char str[], unsigned short n,
                       unsigned char x,unsigned char y,
                       unsigned char color);

extern void _low_put_chars(char c,unsigned short count, unsigned char color);
extern void _low_move_xy(unsigned char x, unsigned char y, unsigned char page);

// _low_scroll_screen with count 0 implies fill.
extern void _low_scroll_screen(unsigned char count, unsigned char color,
                              unsigned char x1,unsigned char y1,
                              unsigned char x2, unsigned char y2);
extern char _low_read_char();

void io_move_fix_location() {
    _low_move_xy(IO_CURRENT_X,IO_CURRENT_Y,0);
}

void io_low_scroll_screen(unsigned char count, unsigned char color,
                          unsigned char x1,unsigned char y1,
                          unsigned char x2, unsigned char y2) {
    _low_scroll_screen(count, color, x1, y1, x2, y2);
}

void io_low_put_char(char c, unsigned char color) {
    _low_put_chars(c, 1, color);   
}

char io_low_read_char() {
    return _low_read_char();
}

#endif