#include <lib/utils/color.h>
#include <drivers/disk/disk.h>
#include <lib/utils/output.h>
#include <lib/utils/logging.h>
#include <lib/utils/time.h>

#include "memmgr/tables/gdt.c"

#define GDT_TABLE_SIZE 5
struct GDTEntry gdt_table[GDT_TABLE_SIZE];
struct GDTReference gdtr;

char DIGIT_TO_HEX[] = "0123456789ABCDEF";

extern void enter_protected_mode();
extern void label_exit();

char *get_memdump_8byte(void *address) {
    static char shared_memdump[17];
    for(int i=0;i<8;i++) {
        unsigned char byte = *(char*)address;
        address++;
        shared_memdump[i<<1]     = DIGIT_TO_HEX[byte/16];
        shared_memdump[(i<<1)|1] = DIGIT_TO_HEX[byte%16];
    }
    shared_memdump[16] = '\0';
    return shared_memdump;
}

void load_kernel() {
    print_log("Loading Kernel");
    int err = load_sectors(MEMORY_LOCATION_KERNEL, 0x80, SECTOR_START_KERNEL, SECTOR_COUNT_KERNEL);
    if(err) {
        print_log("Failed to load kernel in memory: %d", err);
        label_exit();
    } else {
        print_log("Kernel loaded at 0x%x: %s...", MEMORY_LOCATION_KERNEL, get_memdump_8byte(MEMORY_LOCATION_KERNEL));
    }
}

void load_static_library() {
    print_log("Loading Static Library");
    int err = load_sectors(MEMORY_STATIC_LIBRARY, 0x80, SECTOR_START_SHARED_LIBRARY, SECTOR_COUNT_SHARED_LIBRARY);
    if(err) {
        print_log("Failed to load static library in memory: %d", err);
        label_exit();
    } else {
        print_log("Static library loaded at 0x%x: %s...", MEMORY_STATIC_LIBRARY, get_memdump_8byte(MEMORY_STATIC_LIBRARY));
    }
}

void entry_stage() {
    move_xy(6, 11);
    set_color_bg(C_BLACK);
    set_color_fg(C_GREEN);
    print_line("C says 'Hello World'");
    set_color_fg(C_WHITE);

    print_log("");
    load_static_library();
    load_kernel();

    populate_gdt_table(&gdtr, gdt_table, GDT_TABLE_SIZE, 0);

    // Enter_protected_mode never returns.
    print_log("Loading GDT Table and entering protected mode");

    // move cursor to end of screen to not bother protected mode.
    set_display_text_xy(TEXT_WINDOW_WIDTH-1, TEXT_WINDOW_HEIGHT-1);
    enter_protected_mode();
    // And thus PC should never reach here :)
}