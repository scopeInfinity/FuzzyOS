#pragma once

#define TEXT_WINDOW_HEIGHT            0x19
#define TEXT_WINDOW_WIDTH             0x50

unsigned char get_display_text_x();
unsigned char get_display_text_y();
void set_display_text_x(unsigned char x);
void set_display_text_y(unsigned char y);
void set_display_text_xy(unsigned char x, unsigned char y);

void io_low_scroll_screen(unsigned char count, unsigned char color,
                          unsigned char x1, unsigned char y1,
                          unsigned char x2, unsigned char y2);

void io_low_put_char(char c, unsigned char color);