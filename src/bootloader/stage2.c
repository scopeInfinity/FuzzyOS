#include <lib/utils/color.h>
#include <drivers/disk/disk.h>
#include <lib/utils/output.h>
#include <lib/utils/logging.h>
#include <lib/utils/time.h>

#include "memmgr/tables/gdt.c"

#define GDT_TABLE_SIZE 5
struct GDTEntry gdt_table[GDT_TABLE_SIZE];

extern struct GDTReference* _low_get_gdtr_address();
extern void enter_protected_mode();
extern void label_exit();

void load_kernel() {
    print_log("Loading Kernel");
    int err = load_sectors(MEMORY_LOCATION_KERNEL, 0x80, SECTOR_START_KERNEL, SECTOR_COUNT_KERNEL);
    if(err) {
        print_log("Failed to load kernel in memory: %d", err);
        label_exit();
    } else {
        print_log("Kernel loaded at 0x%x: %x...", MEMORY_LOCATION_KERNEL, *(int*)MEMORY_LOCATION_KERNEL);
    }
}

void load_static_library() {
    print_log("Loading Static Library");
    int err = load_sectors(MEMORY_STATIC_LIBRARY, 0x80, SECTOR_START_SHARED_LIBRARY, SECTOR_COUNT_SHARED_LIBRARY);
    if(err) {
        print_log("Failed to load static library in memory: %d", err);
        label_exit();
    } else {
        print_log("Static library loaded at 0x%x: %x...", MEMORY_STATIC_LIBRARY, *(int*)MEMORY_STATIC_LIBRARY);
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

    struct GDTReference* gdtr = _low_get_gdtr_address();
    populate_gdt_table(gdtr, gdt_table, GDT_TABLE_SIZE, 0);

    // Enter_protected_mode never returns.
    print_log("Loading GDT Table and entering protected mode");
    enter_protected_mode();
    // And thus PC should never reach here :)
}