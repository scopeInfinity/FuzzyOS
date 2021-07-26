#pragma once

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


void populate_gdt_entry(struct GDTEntry *entry,
    unsigned int base,
    unsigned int limit,  // 20 bits
    unsigned char flags, //  4 bits
    unsigned char access_byte
    );