#include <lib/utils/color.h>
#include <drivers/disk/disk.h>
#include <lib/utils/output.h>
#include <lib/utils/logging.h>
#include <lib/utils/time.h>

#define GDT_TABLE_SIZE 7

#pragma pack(push, 1)
struct GDTReference {
    unsigned short size;
    unsigned int base_address;
};
struct GDTEntry {
    unsigned short limit0;
    unsigned short base0;
    unsigned char base1;
    unsigned char access_byte;
    unsigned char flags_limit1;
    unsigned char base2;
};
#pragma pack(pop)

struct GDTReference *gdtr;
struct GDTEntry gdt_table[GDT_TABLE_SIZE];

void populate_gct_entry(struct GDTEntry *entry,
    unsigned int base,
    unsigned int limit,  // 20 bits
    unsigned char flags, //  4 bits
    unsigned char access_byte
    ) {
    entry->base0 = (base&0x0000FFFF);
    entry->base1 = (base&0x00FF0000)>>16;
    entry->base2 = (base&0xFF000000)>>24;
    entry->limit0 = (limit&0x0FFFF);
    entry->flags_limit1 = (flags<<4)|((limit&0xF0000)>>16);
    entry->access_byte = access_byte;
}

extern int _low_get_gdtr_address();
extern void enter_protected_mode();
extern void label_exit();

void populate_gdt_table() {
    print_log("Populating GDT Table at 0x%d", gdt_table);

    // NULL selector
    populate_gct_entry(
        &gdt_table[0],
        0,0,0,0);
    // Kernel Code Segment Selector
    populate_gct_entry(
        &gdt_table[1],
        MEMORY_LOCATION_KERNEL, MEMORY_LOCATION_KERNEL+0xFFFF,
        0b0100,  // 32-bit protected mode
        0x9a);
    // Kernel Data Segment Selector
    populate_gct_entry(
        &gdt_table[2],
        MEMORY_LOCATION_KERNEL, MEMORY_LOCATION_KERNEL+0xFFFF,
        0b0100,  // 32-bit protected mode
        0x92);
    // Absolute Code Segment Selector
    populate_gct_entry(
        &gdt_table[3],
        0, 0xfffff,
        0b0000,  // 16-bit protected mode
        0x9a);
    // Absolute Data Segment Selector
    populate_gct_entry(
        &gdt_table[4],
        0, 0xfffff,
        0b0000,  // 16-bit protected mode
        0x92);
    // Application Code Segment Selector
    populate_gct_entry(
        &gdt_table[5],
        MEMORY_LOCATION_APP, MEMORY_LOCATION_APP+0xFFFF,
        0b0100,  // 32-bit protected mode
        0x9a);
    // Application Data Segment Selector
    populate_gct_entry(
        &gdt_table[6],
        MEMORY_LOCATION_APP, MEMORY_LOCATION_APP+0xFFFF,
        0b0100,  // 32-bit protected mode
        0x92);


    gdtr = (struct GDTReference*)_low_get_gdtr_address();
    gdtr->base_address = (int)gdt_table;
    gdtr->size = (sizeof(gdt_table));

    // Print the table and table addresse.
    print_log("GDTR: 0x%x; base address: 0x%x, size: %d",
        (int)gdtr, gdtr->base_address, gdtr->size);
    int gdt_entries = gdtr->size/sizeof(struct GDTEntry);
    for(int i=0;i<gdt_entries;i++) {
        print_log("  GDT Entry %d: %x%x",
            i,
            *(int*)(8*i+(int)gdt_table),
            *(int*)(8*i+4+(int)gdt_table));
    }
}

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
    populate_gdt_table();

    // Enter_protected_mode never returns.
    print_log("Entering protected mode");
    enter_protected_mode();
    // And thus PC should never reach here :)
}