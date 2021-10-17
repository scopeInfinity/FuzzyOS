#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <drivers/display/text_mode.h>
#include <lib/utils/basic.h>
#include <lib/utils/color.h>

void flush_screen();
void move_x(unsigned char x);
void move_y(unsigned char y);
void move_xy(unsigned char x, unsigned char y);
void move_x_diff(char dx);
void move_y_diff(char dy);
void move_xy_diff(char dx, char dy);

void print_rectangle(unsigned char x1, unsigned char y1, unsigned char x2,
                     unsigned char y2);

void scroll(char count, unsigned char x1, unsigned char y1, unsigned char x2,
            unsigned char y2);

void print_char(char c);
void print_hex_nibble(unsigned char x);
void print_hex_byte(unsigned char x);
void print_hex_short(unsigned short x);
void print_hex_int(unsigned int x);
void print_line(const char *str);
void print_memory_hex(const char *str, unsigned short count);
void print_int(int x);

void printf_low(const char *strfmt, int *va_base);

#ifdef __cplusplus
}
#endif