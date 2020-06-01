#ifndef __LIB_SYSCALLS_IO
#define __LIB_SYSCALLS_IO

#include <lib/syscalls/basic.h>
#include <lib/syscalls/color.h>

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

#define _move_fix_location() {_low_move_xy(IO_CURRENT_X,IO_CURRENT_Y,0);}

void move_x(unsigned char x) {
    IO_CURRENT_X=x;
    _move_fix_location();
}

void move_y(unsigned char y) {
    IO_CURRENT_Y=y;
    _move_fix_location();
}

void move_xy(unsigned char x, unsigned char y) {
    IO_CURRENT_X=x;
    IO_CURRENT_Y=y;
    _move_fix_location();
}

void move_x_diff(unsigned char dx) {
    IO_CURRENT_X+=dx;
    _move_fix_location();
}

void move_y_diff(unsigned char dy) {
    IO_CURRENT_Y+=dy;
    _move_fix_location();
}

void move_xy_diff(unsigned char dx, unsigned char dy) {
    IO_CURRENT_X+=dx;
    IO_CURRENT_Y+=dy;
    _move_fix_location();
}

void print_rectangle(unsigned char x1,unsigned char y1,
                     unsigned char x2, unsigned char y2) {
    _low_scroll_screen(0, IO_CURRENT_COLOR, x1, y1, x2, y2);    
}

void scroll(unsigned char count,
            unsigned char x1,unsigned char y1,
            unsigned char x2, unsigned char y2) {
    _low_scroll_screen(count, IO_CURRENT_COLOR, x1, y1, x2, y2);    
}

void print_char(char c) {
    switch(c) {
        case '\n':
            if(IO_CURRENT_Y==WINDOW_HEIGHT) {
                scroll(1, 0,0,WINDOW_WIDTH, WINDOW_HEIGHT);
            } else {
                move_y_diff(1);
            }
            move_x(0);
            break;
        default:
            _low_put_chars(c,1, IO_CURRENT_COLOR);
            move_xy_diff(1, 0);
    }
}

void print_line(char *str) {
    while((*str)!='\0') {
        print_char(*str);
        str++;
    }
}

void print_int(int x) {
    int is_negative = 0;
    int tailing_zero = 0;
    if(x<0) {
        is_negative = 1;
        x=-x;
    }
    int rev = 0;
    while(x && x%10==0) {
        x/=10;
        tailing_zero++;
    }
    if(x==0) {
        // If the initial number is zero, add one here.
        tailing_zero++;
    }
    while(x) {
        rev = rev*10 + x%10;
        x/=10;
    }
    if(is_negative) {
        print_char('-');
    }
    while(rev) {
        print_char((char)(rev%10+'0'));
        rev/=10;
    }
    while(tailing_zero) {
        print_char('0');
        tailing_zero--;
    }
}

char getch() {
    return _low_read_char();
}

void read_line(char *str) {
    int i = 0;
    while(1) {
        str[i]=_low_read_char();
        if(str[i]=='\r') {
            str[i]='\0';
            print_char('\n');
            break;
        }
        print_char(str[i]);
        i++;
    }
}

#endif