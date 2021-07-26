#pragma once

extern void populate_idt_entry_32bit(int id,
    unsigned int address,
    unsigned char dpl, // 2-bit
    int is_trap
    );