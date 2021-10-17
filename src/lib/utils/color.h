#pragma once

#define C_BLACK 0x0
#define C_BLUE 0x1
#define C_GREEN 0x2
#define C_CYAN 0x3
#define C_RED 0x4
#define C_MAGENTA 0x5
#define C_BROWN 0x6
#define C_LIGHT_GRAY 0x7
#define C_DARK_GRAY 0x8
#define C_LIGHT_BLUE 0x9
#define C_LIGHT_GREEN 0xA
#define C_LIGHT_CYAN 0xB
#define C_LIGHT_RED 0xC
#define C_LIGHT_MAGENTA 0xD
#define C_YELLOW 0xE
#define C_WHITE 0xF

#define make_color(fg, bg) ((unsigned char)((fg) + ((bg) << 4)))

unsigned char get_color_fgbg();
void set_color_fgbg(unsigned char color);
void set_color_fg(unsigned char color);
void set_color_bg(unsigned char color);
