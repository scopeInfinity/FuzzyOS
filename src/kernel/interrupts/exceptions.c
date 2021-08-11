#include <fuzzy/kernel/panic.h>

extern void _interrupt_handler_0x00_exception();
extern void _interrupt_handler_0x01_exception();
extern void _interrupt_handler_0x02_exception();
extern void _interrupt_handler_0x03_exception();
extern void _interrupt_handler_0x04_exception();
extern void _interrupt_handler_0x05_exception();
extern void _interrupt_handler_0x06_exception();
extern void _interrupt_handler_0x07_exception();
extern void _interrupt_handler_0x08_exception();
extern void _interrupt_handler_0x09_exception();
extern void _interrupt_handler_0x0A_exception();
extern void _interrupt_handler_0x0B_exception();
extern void _interrupt_handler_0x0C_exception();
extern void _interrupt_handler_0x0D_exception();
extern void _interrupt_handler_0x0E_exception();
extern void _interrupt_handler_0x0F_exception();
extern void _interrupt_handler_0x10_exception();
extern void _interrupt_handler_0x11_exception();
extern void _interrupt_handler_0x12_exception();
extern void _interrupt_handler_0x13_exception();
extern void _interrupt_handler_0x14_exception();
extern void _interrupt_handler_0x15_exception();
extern void _interrupt_handler_0x16_exception();
extern void _interrupt_handler_0x17_exception();
extern void _interrupt_handler_0x18_exception();
extern void _interrupt_handler_0x19_exception();
extern void _interrupt_handler_0x1A_exception();
extern void _interrupt_handler_0x1B_exception();
extern void _interrupt_handler_0x1C_exception();
extern void _interrupt_handler_0x1D_exception();
extern void _interrupt_handler_0x1E_exception();
extern void _interrupt_handler_0x1F_exception();

// https://en.wikipedia.org/wiki/Interrupt_descriptor_table
void interrupt_handler_0x00_0x1F_exception(int id) {
    PANIC(id, "[hw_exception] triggered");
}


void interrupt_register_0x00_0x1F_exceptions() {
    print_log("[interrupts] register 0x00-0x1F exceptions");
    populate_idt_entry_32bit(0x00, (unsigned int)_interrupt_handler_0x00_exception, 0, 0);
    populate_idt_entry_32bit(0x01, (unsigned int)_interrupt_handler_0x01_exception, 0, 0);
    populate_idt_entry_32bit(0x02, (unsigned int)_interrupt_handler_0x02_exception, 0, 0);
    populate_idt_entry_32bit(0x03, (unsigned int)_interrupt_handler_0x03_exception, 0, 0);
    populate_idt_entry_32bit(0x04, (unsigned int)_interrupt_handler_0x04_exception, 0, 0);
    populate_idt_entry_32bit(0x05, (unsigned int)_interrupt_handler_0x05_exception, 0, 0);
    populate_idt_entry_32bit(0x06, (unsigned int)_interrupt_handler_0x06_exception, 0, 0);
    populate_idt_entry_32bit(0x07, (unsigned int)_interrupt_handler_0x07_exception, 0, 0);
    populate_idt_entry_32bit(0x08, (unsigned int)_interrupt_handler_0x08_exception, 0, 0);
    populate_idt_entry_32bit(0x09, (unsigned int)_interrupt_handler_0x09_exception, 0, 0);
    populate_idt_entry_32bit(0x0A, (unsigned int)_interrupt_handler_0x0A_exception, 0, 0);
    populate_idt_entry_32bit(0x0B, (unsigned int)_interrupt_handler_0x0B_exception, 0, 0);
    populate_idt_entry_32bit(0x0C, (unsigned int)_interrupt_handler_0x0C_exception, 0, 0);
    populate_idt_entry_32bit(0x0D, (unsigned int)_interrupt_handler_0x0D_exception, 0, 0);
    populate_idt_entry_32bit(0x0E, (unsigned int)_interrupt_handler_0x0E_exception, 0, 0);
    populate_idt_entry_32bit(0x0F, (unsigned int)_interrupt_handler_0x0F_exception, 0, 0);
    populate_idt_entry_32bit(0x10, (unsigned int)_interrupt_handler_0x10_exception, 0, 0);
    populate_idt_entry_32bit(0x11, (unsigned int)_interrupt_handler_0x11_exception, 0, 0);
    populate_idt_entry_32bit(0x12, (unsigned int)_interrupt_handler_0x12_exception, 0, 0);
    populate_idt_entry_32bit(0x13, (unsigned int)_interrupt_handler_0x13_exception, 0, 0);
    populate_idt_entry_32bit(0x14, (unsigned int)_interrupt_handler_0x14_exception, 0, 0);
    populate_idt_entry_32bit(0x15, (unsigned int)_interrupt_handler_0x15_exception, 0, 0);
    populate_idt_entry_32bit(0x16, (unsigned int)_interrupt_handler_0x16_exception, 0, 0);
    populate_idt_entry_32bit(0x17, (unsigned int)_interrupt_handler_0x17_exception, 0, 0);
    populate_idt_entry_32bit(0x18, (unsigned int)_interrupt_handler_0x18_exception, 0, 0);
    populate_idt_entry_32bit(0x19, (unsigned int)_interrupt_handler_0x19_exception, 0, 0);
    populate_idt_entry_32bit(0x1A, (unsigned int)_interrupt_handler_0x1A_exception, 0, 0);
    populate_idt_entry_32bit(0x1B, (unsigned int)_interrupt_handler_0x1B_exception, 0, 0);
    populate_idt_entry_32bit(0x1C, (unsigned int)_interrupt_handler_0x1C_exception, 0, 0);
    populate_idt_entry_32bit(0x1D, (unsigned int)_interrupt_handler_0x1D_exception, 0, 0);
    populate_idt_entry_32bit(0x1E, (unsigned int)_interrupt_handler_0x1E_exception, 0, 0);
    populate_idt_entry_32bit(0x1F, (unsigned int)_interrupt_handler_0x1F_exception, 0, 0);
}
