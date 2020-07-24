#include <lib/utils/color.h>
#include <lib/utils/disk.h>
#include <lib/utils/output.h>
#include <lib/utils/time.h>

char message_welcome[] = "C says 'Hello World'";
char message_dashboard[] = "Opening App 'Dashboard'";
char message_kernel_loading[] = "Loading Kernel....";
char message_calc_loading[] = "Loading Calc....";
char message_protected_mode[] = "Enabling Protected Mode...";

#define GDT_TABLE_SIZE 3

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

// Issue#2: gdt_table must immediately after gdtr.
struct GDTReference gdtr;
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

extern void enter_protected_mode(int gdtr_address);
extern void label_exit();

int populate_gdt_table() {
    // Assumption DS = 0
    // Populate simple overlapping code and data segment.

    // Kernel Memory Location: 0x100000

    populate_gct_entry(
        &gdt_table[0],
        0,0,0,0);
    // Kernel Code Segment Selector
    populate_gct_entry(
        &gdt_table[1],
        // 0x00000000,0x0fffffff,
        KERNEL_MEMORY_LOCATION, 0x0fffffff,
        0b0100,  // 32-bit protected mode
        0x9a);
    // Kernel Data Segment Selector
    populate_gct_entry(
        &gdt_table[2],
        // 0x00000000,0x0fffffff,
        KERNEL_MEMORY_LOCATION, 0x0fffffff,
        0b0100,  // 32-bit protected mode
        0x92);

    // Issue#2: For some reason gdtr.base_address assigned is not
    // being respected later on. So, as a bad workaround hardcoding
    // base_address as &gdtr+8 in stage2.asm.
    gdtr.base_address = (int)gdt_table;
    gdtr.size = (sizeof(gdt_table));

    // Print the table and table addresse.
    move_xy(8,15);
    print_int((int)&gdtr);
    move_xy(8,16);
    print_int(gdtr.base_address);
    move_xy(8,17);
    print_int(gdtr.size);
    for(int i=0;i<GDT_TABLE_SIZE;i++) {
        move_xy(8,18+i);
        print_memory_hex(8*i+(char*)&gdt_table, 8);
    }
    return (int)&gdtr;
}

void load_kernel() {
    // As we are in real mode with DS as 0
    // KERNEL_MEMORY_LOCATION should be within 16 bit for now.
    int err = load_sectors(KERNEL_MEMORY_LOCATION, 0x80, DISK_KERNEL_SECTOR_START, DISK_KERNEL_SECTOR_COUNT);
    if(err) {
        print_line("Failed to load kernel in memory: ");
        print_int(err);
        label_exit();
    } else {
        print_memory_hex((char*)KERNEL_MEMORY_LOCATION, 16);
    }
}

void load_calc() {
    int err = load_sectors(0x2000, 0x80, 27, 25);
    if(err) {
        print_line("Failed to load calc in memory.");
        print_int(err);
        label_exit();
    } else {
        print_memory_hex((char*)0x2000, 16);
    }
}

void entry_stage() {
    move_xy(6, 11);
    set_color_bg(C_BLACK);
    set_color_fg(C_GREEN);
    print_line(message_welcome);
    set_color_fg(C_WHITE);
    move_xy(6, 12);
    print_line(message_kernel_loading);
    load_kernel();
    move_xy(6, 13);
    print_line(message_calc_loading);
    load_calc();
    move_xy(6, 14);
    print_line(message_protected_mode);
    int gdtr_address = populate_gdt_table();
    // Enter_protected_mode never returns.
    enter_protected_mode(gdtr_address);
    // And thus PC should never reach here :)
}