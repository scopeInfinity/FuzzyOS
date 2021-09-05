#include<fuzzy/drivers/display/vga/graphics.h>
#include<fuzzy/kernel/process/process.h>
#include<fuzzy/memmgr/tables/gdt.h>
#include<fuzzy/memmgr/layout.h>

#include <stddef.h>

// FIX: large buffer size, looks like we need to move kernel.
static uint16_t buffer[GRAPHICS_HEIGHT][GRAPHICS_WIDTH] = {0};

void graphics_flush() {
    kernel_memncpy_absolute(
        GDT_ABS32_DS, // dst ds
        MEMORY_VGA_GRAPHICS_ADDRESS, // dst address
        GDT_KERNEL_DS, // src ds
        buffer, // src address
        MEMORY_VGA_GRAPHICS_SIZE); // FIX: doesn't match
}
