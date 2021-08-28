#include <fuzzy/memmgr/layout.h>

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
extern unsigned short call_int_0x15(unsigned short ax);

void enable_a20() {
    int ax;
    ax = call_int_0x15(0x2403);
    if (!ax) {
        print_log("BIOS A20-gate not supported");
        label_exit();
    }

    ax = call_int_0x15(0x2402);
    if (ax==1) {
        print_log("BIOS A20-gate already enabled");
        return;
    }

    ax = call_int_0x15(0x2401);
    if (!ax) {
        print_log("BIOS A20-gate enabling attempt failed");
        label_exit();
    }

    print_log("BIOS A20-gate enabled");
}

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
    int err = load_sectors(MEMORY_KERNEL_LOCATION, 0x80, SECTOR_START_KERNEL, SECTOR_COUNT_KERNEL);
    if(err) {
        print_log("Failed to load kernel in memory: %d", err);
        label_exit();
    } else {
        print_log("Kernel loaded at 0x%x: %s...", MEMORY_KERNEL_LOCATION, get_memdump_8byte(MEMORY_KERNEL_LOCATION));
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

    enable_a20();
    populate_gdt_table(&gdtr, gdt_table, GDT_TABLE_SIZE, 0);

    // Enter_protected_mode never returns.
    print_log("Loading GDT Table and entering protected mode");

    // move cursor to end of screen to not bother protected mode.
    set_display_text_xy(TEXT_WINDOW_WIDTH-1, TEXT_WINDOW_HEIGHT-1);
    enter_protected_mode();
    // And thus PC should never reach here :)
}