#include<fuzzy/drivers/display/vga/graphics.h>
#include<fuzzy/kernel/process/process.h>
#include<fuzzy/memmgr/tables/gdt.h>
#include<fuzzy/memmgr/layout.h>
#include<fuzzy/real_mode/client.h>

#include <stddef.h>

int graphics_switchto_320x200x256() {
    uint8_t video_mode_flag = 0x13;  // 320x200x256
    real_mode_client(
        0x10, // interrupt number
        (0x00<<8) | video_mode_flag,
        0,  // do not care
        0,  // do not care
        0,  // do not care
        0 // do not care
    );
    return 0;
 }

int graphics_write_320x200x256(int user_ds, uint8_t *__us_buffer) {
    const int size = 320*200*1;
    kernel_memncpy_absolute(
        GDT_ABS32_DS, // dst ds
        MEMORY_VGA_GRAPHICS_ADDRESS, // dst address
        user_ds, // src ds
        __us_buffer, // src address
        size);
    return 0;
}
