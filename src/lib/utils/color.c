#include <lib/utils/color.h>

unsigned char IO_CURRENT_FG = 0;
unsigned char IO_CURRENT_BG = 0;
unsigned char IO_CURRENT_COLOR = 0;

void _compute_color() {
    IO_CURRENT_COLOR = make_color(IO_CURRENT_FG, IO_CURRENT_BG);
}

unsigned char get_color_fgbg() { return IO_CURRENT_COLOR; }

void set_color_fgbg(unsigned char color) { IO_CURRENT_COLOR = color; }

void set_color_fg(unsigned char color) {
    IO_CURRENT_FG = color;
    _compute_color();
}

void set_color_bg(unsigned char color) {
    IO_CURRENT_BG = color;
    _compute_color();
}