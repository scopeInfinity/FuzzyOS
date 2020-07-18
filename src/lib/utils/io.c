#include <lib/utils/basic.h>
#include <lib/utils/color.h>
#include <lib/utils/io.h>
#include <drivers/display/text_mode.h>

void move_x(unsigned char x) {
    set_display_text_x(x);
}

void move_y(unsigned char y) {
    set_display_text_y(y);
}

void move_xy(unsigned char x, unsigned char y) {
    set_display_text_xy(x, y);
}

void move_x_diff(unsigned char dx) {
    set_display_text_x(get_display_text_x()+dx);
}

void move_y_diff(unsigned char dy) {
    set_display_text_y(get_display_text_y()+dy);
}

void move_xy_diff(unsigned char dx, unsigned char dy) {
    int x = get_display_text_x()+dx;
    int y = get_display_text_y()+dy;
    set_display_text_xy(x, y);
}

void print_rectangle(unsigned char x1,unsigned char y1,
                     unsigned char x2, unsigned char y2) {
    io_low_scroll_screen(0, get_color_fgbg(), x1, y1, x2, y2);
}

void scroll(unsigned char count,
            unsigned char x1,unsigned char y1,
            unsigned char x2, unsigned char y2) {
    io_low_scroll_screen(count, get_color_fgbg(), x1, y1, x2, y2);
}

void print_char(char c) {
    switch(c) {
        case '\n':
            if (get_display_text_y()>TEXT_WINDOW_HEIGHT) {
                scroll(1, 0,0,TEXT_WINDOW_WIDTH, TEXT_WINDOW_HEIGHT);
            } else {
                move_y_diff(1);
            }
            move_x(0);
            break;
        default:
            io_low_put_char(c, get_color_fgbg());
            move_xy_diff(1, 0);
    }
}
char digit_to_hex[] = "0123456789ABCDEF";

void print_hex_nibble(unsigned char x) {
    if(x<10) {
        print_char((char)(x+'0'));
    } else {
        print_char((char)(x+('A'-10)));
    }
}

void print_hex_byte(unsigned char x) {
    print_hex_nibble((x>>4)&0xF);
    print_hex_nibble(x&0xF);
}

void print_hex_short(unsigned short x) {
    print_hex_byte(x>>8);
    print_hex_byte(x&0xFF);
}

void print_hex_int(unsigned int x) {
    print_hex_short(x>>16);
    print_hex_short(x&0xFFFF);
}

void print_line(const char *str) {
    while((*str)!='\0') {
        print_char(*str);
        str++;
    }
}

void print_memory_hex(const char *str,unsigned short count) {
    while(count) {
        print_hex_byte(*str);
        str++;
        count--;
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
    return io_low_read_char();
}

void read_line(char *str) {
    int i = 0;
    while(1) {
        str[i]=io_low_read_char();
        if(str[i]=='\r') {
            str[i]='\0';
            print_char('\n');
            break;
        }
        print_char(str[i]);
        i++;
    }
}