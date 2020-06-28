#pragma once

#include <lib/utils/basic.h>
#include <lib/utils/color.h>
#include <drivers/display/text_mode.h>

void move_x(unsigned char x);
void move_y(unsigned char y);
void move_xy(unsigned char x, unsigned char y);
void move_x_diff(unsigned char dx);
void move_y_diff(unsigned char dy);
void move_xy_diff(unsigned char dx, unsigned char dy);

void print_rectangle(unsigned char x1,unsigned char y1,
                     unsigned char x2, unsigned char y2);

void scroll(unsigned char count,
            unsigned char x1,unsigned char y1,
            unsigned char x2, unsigned char y2);

void print_char(char c);
void print_hex_nibble(unsigned char x);
void print_hex_byte(unsigned char x);
void print_line(const char *str);
void print_memory_hex(const char *str,unsigned short count);
void print_int(int x);

char getch();
void read_line(char *str);