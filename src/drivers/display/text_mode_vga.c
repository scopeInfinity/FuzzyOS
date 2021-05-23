#include <drivers/display/text_mode.h>

extern void _low_put_char(char c, unsigned char color, unsigned int xy);

extern void _low_vga_copy_step(unsigned int xy1, unsigned int xy2, unsigned int count);

extern _low_flush(unsigned short *buffer, int count);

static int location_xy = 0;

int IO_CURRENT_X = 0;
int IO_CURRENT_Y = 0;
unsigned short buffer[TEXT_WINDOW_HEIGHT*TEXT_WINDOW_WIDTH] = {0};

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
            int d_index = r*TEXT_WINDOW_WIDTH+x1;
            for (int c = x1; c <= x2; ++c) {
                buffer[d_index++]=(((unsigned short)color)<<8)|' ';
            }
        }
    } else if (count > 0) {
        // Not yet tested.
        int width = x2-x1+1;
        for (int r_dest=y1,r_src = y1+count; r_src <= y2; r_src++,r_dest++) {
            int s_index = r_src*TEXT_WINDOW_WIDTH+x1;
            int d_index = r_dest*TEXT_WINDOW_WIDTH+x1;
            for (int j = x1; j <= x2; ++j) {
                buffer[d_index++]=buffer[s_index++];
            }
        }
    } else {
        // Not yet tested.
        int width = x2-x1+1;
        for (int r_dest=y2,r_src = y2+count; r_src >= y1; r_src--,r_dest--) {
            int s_index = r_src*TEXT_WINDOW_WIDTH+x1;
            int d_index = r_dest*TEXT_WINDOW_WIDTH+x1;
            for (int j = x1; j <= x2; ++j) {
                buffer[d_index++]=buffer[s_index++];
            }
        }
    }
    io_low_flush();
}

void io_low_put_char(char c, unsigned char color) {
    _low_put_char(c,color, location_xy);
    buffer[location_xy]=(((unsigned short)color)<<8)|c;
}

void io_low_flush() {
    _low_flush(buffer, sizeof(buffer)/2);
}