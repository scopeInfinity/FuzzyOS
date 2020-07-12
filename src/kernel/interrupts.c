#define IDT_SIZE 1

struct IDTEntry {
    unsigned short something0;
    unsigned short something1;
    unsigned short something2;
    unsigned short something3;
    unsigned short something4;
    unsigned short something5;
    unsigned short something6;
    unsigned short something7;
};

struct IDTReference {
    unsigned short size;
    unsigned int base_address;
};
struct IDTReference idtr;
struct IDTEntry idt_table[IDT_SIZE];

void populate_ict_entry(struct IDTEntry *entry) {
}

int populate_idt_table() {
    return (int)&idtr;
}