#ifndef __LIB_SYSCALLS_IO_INTERFACE_PROTECTED
#define __LIB_SYSCALLS_IO_INTERFACE_PROTECTED
#include <lib/syscalls/basic.h>
#include <lib/syscalls/io_interface.h>

extern void _low_put_char(char c, unsigned char color, unsigned int xy);

extern void _low_vga_copy_step(unsigned int xy1, unsigned int xy2, unsigned int count);

unsigned int __LIB_SYSCALLS_IO_INTERFACE_PROTECTED_XY = 0;

void io_move_fix_location() {
    __LIB_SYSCALLS_IO_INTERFACE_PROTECTED_XY = IO_CURRENT_X + IO_CURRENT_Y*WINDOW_WIDTH;
}

void io_low_scroll_screen(unsigned char count, unsigned char color,
                          unsigned char x1, unsigned char y1,
                          unsigned char x2, unsigned char y2) {
    if (count == 0) {
        for (int r = y1; r <= y2; ++r) {
            int index = r*WINDOW_WIDTH;
            for (int c = x1; c <= x2; ++c, index++) {
                _low_put_char(' ', color, index);
            }
        }
    } else if (count > 0 ) {
        // Not yet tested.
        int width = x2-x1+1;
        for (int r1=y1,r2 = y1+count; r2 <= y2; r1++,r2++) {
            _low_vga_copy_step(
                r2*WINDOW_WIDTH + x1,
                r1*WINDOW_WIDTH + x1,
                width
                );
            io_low_scroll_screen(0,color,
                x1, y2-count,
                x2, y2);
        }
    } else {
        // Not yet tested.
        int width = x2-x1+1;
        for (int r1=y2,r2 = y2-count; r2 >= y1; r1--,r2--) {
            _low_vga_copy_step(
                r2*WINDOW_WIDTH + x1,
                r1*WINDOW_WIDTH + x1,
                width
                );
            io_low_scroll_screen(0,color,
                x1, y1,
                x2, y1+count);
        }
    }
}

void io_low_put_char(char c, unsigned char color) {
    _low_put_char(c,color, __LIB_SYSCALLS_IO_INTERFACE_PROTECTED_XY);
}

char io_low_read_char() {
    // Not implemented!
    while(1);
    return 0;
}

#endif