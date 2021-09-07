#include <fuzzy/kernel/syscall/graphics.h>
#include <fuzzy/drivers/display/vga/graphics.h>

#include <graphics.h>

static int _graphics_init() {
    // switch to 320x200x256 mode
    return graphics_switchto_320x200x256();
}

static int _graphics_close() {
    return 0;
}

static int _graphics_copybuffer(int user_ds, void *_us_buffer) {
    return graphics_write_320x200x256(user_ds, _us_buffer);
}

int syscall_4_graphics(int operation, int a1, int a2, int a3, int user_ds) {
    switch (operation) {
        case SYSCALL_GRAPHICS_INITGRAPH:
            return _graphics_init();
        case SYSCALL_GRAPHICS_CLOSEGRAPH:
            return _graphics_close();
        case SYSCALL_GRAPHICS_COPYBUFFER:
            return _graphics_copybuffer(user_ds, (void *)a1);
    }
    return -1;
}