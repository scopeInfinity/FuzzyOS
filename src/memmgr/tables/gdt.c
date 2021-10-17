#include <fuzzy/memmgr/layout.h>
#include <fuzzy/memmgr/tables/gdt.h>

#include <lib/utils/logging.h>

int get_gdt_baseaddress(struct GDTEntry gdt_table[], unsigned int table_size,
                        int entry_id) {
    if (entry_id >= 0 && entry_id < table_size) {
        unsigned int base0 = gdt_table[entry_id].base0;
        unsigned int base1 = gdt_table[entry_id].base1;
        base1 <<= 16;
        unsigned int base2 = gdt_table[entry_id].base2;
        base2 <<= 24;
        return base2 | base1 | base0;
    }
    return -1;
}

void populate_gdt_entry(struct GDTEntry *entry, unsigned int base,
                        unsigned int limit, // 32 bit with 4k granularity
                        unsigned char access_byte, int is_32_bit_selector) {
    const int is_4k_granularity = 1;

    char flags = 0b0000; // 4 bits
    if (is_4k_granularity)
        flags |= 0b1000;
    if (is_32_bit_selector)
        flags |= 0b0100;

    if (is_4k_granularity) {
        limit >>= 12;
    }

    entry->base0 = (base & 0x0000FFFF);
    entry->base1 = (base & 0x00FF0000) >> 16;
    entry->base2 = (base & 0xFF000000) >> 24;
    entry->limit0 = (limit & 0x0FFFF);
    entry->flags_limit1 = (flags << 4) | ((limit & 0xF0000) >> 16);
    entry->access_byte = access_byte;
}

void populate_gdt_table(struct GDTReference *gdtr, struct GDTEntry gdt_table[],
                        int entries_count, int ds_fix) {
    // Populate the allocated GDT Table and GDT Reference.
    // gdtr          : Reference to the GDT Table to be consumed by lgdt.
    // gdt_table     : GDT Table.
    // entries_count : Allocated number of entries for GDT Table.
    // ds_fix        : Data Segment fix to translate gdt_table relative
    //                 address to absolute address.

    print_log("Populating GDT Table at 0x%d", gdt_table);

    // NULL selector
    populate_gdt_entry(&gdt_table[GDT_STD_SELECTOR_NULL], 0, 0, 0,
                       !GDT_ENTRY_FLAG_32_BIT_SELECTOR);
    // Kernel Code Segment Selector
    populate_gdt_entry(&gdt_table[GDT_STD_SELECTOR_KERNEL_CS],
                       MEMORY_KERNEL_LOCATION, MEMORY_KERNEL_SIZE - 1, 0x9a,
                       GDT_ENTRY_FLAG_32_BIT_SELECTOR // 32-bit protected mode
    );
    // Kernel Data Segment Selector
    populate_gdt_entry(&gdt_table[GDT_STD_SELECTOR_KERNEL_DS],
                       MEMORY_KERNEL_LOCATION, MEMORY_KERNEL_SIZE - 1, 0x92,
                       GDT_ENTRY_FLAG_32_BIT_SELECTOR // 32-bit protected mode
    );
    // Absolute Code Segment Selector
    populate_gdt_entry(&gdt_table[GDT_STD_SELECTOR_ABS16_CS], 0, 0xfffff, 0x9a,
                       !GDT_ENTRY_FLAG_32_BIT_SELECTOR // 16-bit protected mode
    );
    // Absolute Data Segment Selector
    populate_gdt_entry(&gdt_table[GDT_STD_SELECTOR_ABS16_DS], 0, 0xfffff, 0x92,
                       !GDT_ENTRY_FLAG_32_BIT_SELECTOR // 16-bit protected mode
    );
    // NOT USING &gdt_table[5] for now.
    // Absolute Data Segment Selector
    populate_gdt_entry(&gdt_table[GDT_STD_SELECTOR_ABS32_DS], 0, 0xffffffff,
                       0x92,
                       GDT_ENTRY_FLAG_32_BIT_SELECTOR // 32-bit protected mode
    );
    // Ensure GDT_STD_SIZE = last sector entry+1

    gdtr->base_address = ds_fix + (int)gdt_table;
    gdtr->size = (entries_count * sizeof(struct GDTEntry));

    // Log table and table address.
    print_log("GDTR: 0x%x; base address: 0x%x, size: %d", (int)gdtr,
              gdtr->base_address, gdtr->size);

    if (LOG_VVV) {
        int gdt_entries = gdtr->size / sizeof(struct GDTEntry);
        for (int i = 0; i < gdt_entries; i++) {
            print_log("  GDT Entry %d: %x%x", i,
                      *(int *)(8 * i + (int)gdt_table),
                      *(int *)(8 * i + 4 + (int)gdt_table));
        }
    }
}

void load_gdt_table(struct GDTReference *gdtr) {
    print_log("Loading GDT Table: 0x%x; base address: 0x%x, size: %d",
              (int)gdtr, gdtr->base_address, gdtr->size);
    __asm__("mov %0, %%eax\n\t"
            "lgdt (%%eax)"
            :
            : "r"(gdtr));
}