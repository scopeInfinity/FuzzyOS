#include <drivers/display/text_mode.h>

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

unsigned char IO_CURRENT_X = 0;
unsigned char IO_CURRENT_Y = 0;

unsigned char get_display_text_x() {
    return IO_CURRENT_X;
}

unsigned char get_display_text_y() {
    return IO_CURRENT_Y;
}

void set_display_text_x(unsigned char x) {
    IO_CURRENT_X = x;
    _low_move_xy(IO_CURRENT_X, IO_CURRENT_Y, 0);
}

void set_display_text_y(unsigned char y) {
    IO_CURRENT_Y = y;
    _low_move_xy(IO_CURRENT_X, IO_CURRENT_Y, 0);
}

void set_display_text_xy(unsigned char x, unsigned char y) {
    IO_CURRENT_X = x;
    IO_CURRENT_Y = y;
    _low_move_xy(IO_CURRENT_X, IO_CURRENT_Y, 0);
}

void io_low_scroll_screen(unsigned char count, unsigned char color,
                          unsigned char x1,unsigned char y1,
                          unsigned char x2, unsigned char y2) {
    _low_scroll_screen(count, color, x1, y1, x2, y2);
}

void io_low_put_char(char c, unsigned char color) {
    _low_put_chars(c, 1, color);
}