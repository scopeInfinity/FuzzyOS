#ifndef __LIB_SYSCALLS_IO
#define __LIB_SYSCALLS_IO

extern void _low_print(char str[], unsigned short n,
                       unsigned char x,unsigned char y,
                       unsigned char color);

extern void _low_put_chars(char c,unsigned short count, unsigned char color);
extern void _low_move_xy(unsigned char x, unsigned char y, unsigned char page);
extern void _low_clear_screen(unsigned char color,
                              unsigned char x1,unsigned char y1,
                              unsigned char x2, unsigned char y2);

unsigned char IO_CURRENT_X = 0;
unsigned char IO_CURRENT_Y = 0;

void move_xy(unsigned char x, unsigned char y) {
    IO_CURRENT_X=x;
    IO_CURRENT_Y=y;
    _low_move_xy(x,y,0);
}

void move_xy_diff(unsigned char dx, unsigned char dy) {
    IO_CURRENT_X+=dx;
    IO_CURRENT_Y+=dy;
    _low_move_xy(IO_CURRENT_X,IO_CURRENT_Y,0);
}

void put_char(char c, unsigned char color) {
    _low_put_chars(c,1, color);
    move_xy_diff(1, 0);
}

void put_chars(char c,unsigned short count, unsigned char color) {
    _low_put_chars(c,count, color);
    move_xy_diff(count, 0);
}

void put_string(char *str, unsigned char color) {
    while((*str)!='\0') {
        put_char(*str, color);
        str++;
    }
}

#endif