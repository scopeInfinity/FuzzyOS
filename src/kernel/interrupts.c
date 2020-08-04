#include <lib/utils/logging.h>

#include "syscall.c"

#define IDT_SIZE 128

extern void interrupt_nohup();
extern void load_idt_table_low(unsigned int idtr_address);

#pragma pack(push, 1)
struct IDTEntry {
    unsigned short offset_0;
    unsigned short selector;
    unsigned char  unused;
    unsigned char  type_attr;
    unsigned short offset_1;
};

struct IDTReference {
    unsigned short size;
    unsigned int base_address;
};
#pragma pack(pop)

struct IDTReference idtr;
struct IDTEntry idt_table[IDT_SIZE];

void populate_idt_entry(int id,
    unsigned short selector,
    unsigned int address,
    unsigned char present,  // 1-bit
    unsigned char dpl, // 2-bit
    unsigned char  storage_segment, // 1-bit
    unsigned char gate_type // 4-bit
    ) {
    struct IDTEntry *entry = &idt_table[id];
    entry->offset_0 = address&0xFFFF;
    entry->offset_1 = (address>>16)&0xFFFF;
    entry->selector = selector;
    entry->unused = 0;
    entry->type_attr = (present<<7) | (dpl<<5) | (storage_segment<<4) | gate_type;
}

void populate_idt_entry_32bit(int id,
    unsigned int address,
    unsigned char dpl, // 2-bit
    int is_trap
    ) {
    populate_idt_entry(
        id,
        0x08,  // kernel code segment selector
        address,
        1,
        dpl,
        0,
        is_trap?0b1111:0b1110
        );
}

extern int syscall_selector_low();
extern int SYSCALL_TABLE[];
int syscall_selector(int id, int arg0, int arg1,int arg2,int arg3) {
    return ((int(*)(int,int,int,int))(SYSCALL_TABLE[id]))(arg0, arg1, arg2, arg3);
}

void populate_and_load_idt_table() {
    print_log("Populating IDT Table");
    for (int i = 0; i < IDT_SIZE; ++i) {
        populate_idt_entry_32bit(i, (unsigned int)interrupt_nohup, 0, 1);
    }
    print_log("  Placed %d no-hub interrupts", IDT_SIZE);
    populate_idt_entry_32bit(0x32, (unsigned int)syscall_selector_low, 0, 1);

    print_log("  Placed custom interrupts (if any)");
    register_syscalls();
    idtr.size = sizeof(struct IDTEntry)*IDT_SIZE;
    idtr.base_address = ((int)idt_table + MEMORY_LOCATION_KERNEL);
    print_log("IDTR: 0x%x; base address: 0x%x, size: %d",
        (int)&idtr, idtr.base_address, idtr.size);
    reload_idt_table();
}

void reload_idt_table() {
    print_log("Loading IDT Table");
    int idtr_address = (int)&idtr;
    load_idt_table_low(idtr_address);
}