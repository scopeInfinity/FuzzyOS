#pragma once

#define TEXT_WINDOW_HEIGHT            0x19
#define TEXT_WINDOW_WIDTH             0x50

int get_display_text_x();
int get_display_text_y();
void set_display_text_x(int x);
void set_display_text_y(int y);
void set_display_text_xy(int x, int y);

void io_low_scroll_screen(char count, unsigned char color,
                          int x1, int y1,
                          int x2, int y2);

void io_low_put_char(char c, unsigned char color);