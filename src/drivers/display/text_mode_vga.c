#include <drivers/display/text_mode.h>

extern void _low_put_char(char c, unsigned char color, unsigned int xy);

extern void _low_vga_copy_step(unsigned int xy1, unsigned int xy2, unsigned int count);

static int location_xy = 0;

int IO_CURRENT_X = 0;
int IO_CURRENT_Y = 0;

int get_display_text_x() {
    return IO_CURRENT_X;
}

int get_display_text_y() {
    return IO_CURRENT_Y;
}

void set_display_text_x(int x) {
    IO_CURRENT_X = x;
    location_xy = IO_CURRENT_X + IO_CURRENT_Y*TEXT_WINDOW_WIDTH;
}

void set_display_text_y(int y) {
    IO_CURRENT_Y = y;
    location_xy = IO_CURRENT_X + IO_CURRENT_Y*TEXT_WINDOW_WIDTH;
}

void set_display_text_xy(int x, int y) {
    IO_CURRENT_X = x;
    IO_CURRENT_Y = y;
    location_xy = IO_CURRENT_X + IO_CURRENT_Y*TEXT_WINDOW_WIDTH;
}

void io_low_scroll_screen(char count, unsigned char color,
                          int x1, int y1,
                          int x2, int y2) {
    if (count == 0) {
        for (int r = y1; r <= y2; ++r) {
            int index = r*TEXT_WINDOW_WIDTH;
            for (int c = x1; c <= x2; ++c, index++) {
                _low_put_char(' ', color, index);
            }
        }
    } else if (count > 0) {
        // Not yet tested.
        int width = x2-x1+1;
        for (int r1=y1,r2 = y1+count; r2 <= y2; r1++,r2++) {
            _low_vga_copy_step(
                r2*TEXT_WINDOW_WIDTH + x1,
                r1*TEXT_WINDOW_WIDTH + x1,
                width
                );
            io_low_scroll_screen(0,color,
                x1, y2-count,
                x2, y2);
        }
    } else {
        // Not yet tested.
        int width = x2-x1+1;
        for (int r1=y2,r2 = y2+count; r2 >= y1; r1--,r2--) {
            _low_vga_copy_step(
                r2*TEXT_WINDOW_WIDTH + x1,
                r1*TEXT_WINDOW_WIDTH + x1,
                width
                );
            io_low_scroll_screen(0,color,
                x1, y1,
                x2, y1+count);
        }
    }
}

void io_low_put_char(char c, unsigned char color) {
    _low_put_char(c,color, location_xy);
}