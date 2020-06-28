#ifndef __LIB_SYSCALLS_IO_INTERFACE
#define __LIB_SYSCALLS_IO_INTERFACE

unsigned char IO_CURRENT_X = 0;
unsigned char IO_CURRENT_Y = 0;

unsigned char get_cursor_x() {
    return IO_CURRENT_X;
}

unsigned char get_cursor_y() {
    return IO_CURRENT_Y;
}

void io_move_fix_location();

void io_low_scroll_screen(unsigned char count, unsigned char color,
                          unsigned char x1,unsigned char y1,
                          unsigned char x2, unsigned char y2);

void io_low_put_char(char c, unsigned char color);

char io_low_read_char();
#endif