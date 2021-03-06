#include <lib/utils/logging.h>

// Min GDT_TABLE_SIZE:    5
// Max GDT_TABLE_SIZE: 8192

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

void populate_gdt_table(
        struct GDTReference* gdtr,
        struct GDTEntry gdt_table[],
        int entries_count,
        int ds_fix) {
    // Populate the allocated GDT Table and GDT Reference.
    // gdtr          : Reference to the GDT Table to be consumed by lgdt.
    // gdt_table     : GDT Table.
    // entries_count : Allocated number of entries for GDT Table.
    // ds_fix        : Data Segment fix to translate gdt_table relative
    //                 address to absolute address.

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

    gdtr->base_address = ds_fix+(int)gdt_table;
    gdtr->size = (entries_count*sizeof(struct GDTEntry));

    // Log table and table address.
    print_log("GDTR: 0x%x; base address: 0x%x, size: %d",
        (int)gdtr, gdtr->base_address, gdtr->size);

    if(LOG_VVV) {
        int gdt_entries = gdtr->size/sizeof(struct GDTEntry);
        for(int i=0;i<gdt_entries;i++) {
            print_log("  GDT Entry %d: %x%x",
                i,
                *(int*)(8*i+(int)gdt_table),
                *(int*)(8*i+4+(int)gdt_table));
        }
    }
}

void load_gdt_table(struct GDTReference* gdtr) {
    print_log("Loading GDT Table: 0x%x; base address: 0x%x, size: %d",
        (int)gdtr, gdtr->base_address, gdtr->size);
    asm("mov %0, %%eax\n\t"
        "lgdt (%%eax)"
        :
        : "r" (gdtr));
}