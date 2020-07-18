#define IDT_SIZE 16

extern void interrupt_nohup();
extern void load_idt_table(unsigned int idtr_address);

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

void populate_and_load_idt_table() {
    for (int i = 0; i < IDT_SIZE; ++i) {
        populate_idt_entry_32bit(i, (unsigned int)interrupt_nohup, 0, 1);
    }
    idtr.size = sizeof(struct IDTEntry)*IDT_SIZE;
    idtr.base_address = ((int)idt_table + KERNEL_MEMORY_LOCATION);

    move_xy(4,4);
    print_line("IDT Reference: ");
    int idtr_address = (int)&idtr;
    print_hex_int(idtr_address);

    move_xy(4,5);
    print_line("IDT Table: ");
    print_hex_int(idtr.base_address);
    print_char(' ');
    print_hex_int(idtr.size);

    load_idt_table(idtr_address);
}