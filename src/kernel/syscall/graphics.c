#include <fuzzy/kernel/syscall/graphics.h>
#include <fuzzy/drivers/display/vga/graphics.h>
#include <fuzzy/memmgr/tables/gdt.h>
#include <graphics.h>

static int _graphics_prev_mode = -1;


static int _graphics_init() {
    if(_graphics_prev_mode!=-1) {
        // already in graphics mode
        return -1;
    }
    _graphics_prev_mode = graphics_get_mode();
    int err = graphics_set_mode(GRAPHIC_MODE_320x200x256);
    return err;
}

static int _graphics_close() {
    if(_graphics_prev_mode==-1) {
        // not in graphics mode
        return -1;
    }
    int err = graphics_set_mode(_graphics_prev_mode);
    // flush text screen
    flush_screen();
    _graphics_prev_mode = -1;
    return err;
}

static int _graphics_copybuffer(int user_ds, void *_us_buffer) {
    uint8_t ARR[10][10];
    for(int i=0;i<10;i++)for(int j=0;j<10;j++) {
        ARR[i][j] = WHITE;
    }
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