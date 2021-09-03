#include <fuzzy/kernel/panic.h>
#include <fuzzy/memmgr/tables/gdt.h>
#include <fuzzy/kernel/process/process.h>

extern struct GDTEntry gdt_table[];

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

void interrupt_handler_0x00_0x1F_exception(int id, int err_code, int ds, int ss, int ip, int cs, int eflag) {
    panic_screen_init();
    unsigned int abs_address =
    print_log("Hardware exception %d (0x%x) triggered", id, id);
    print_log("  Error Code: %x or %x", err_code);
    print_log("  CS    : %x (GDT entry)", cs);
    print_log("  DS    : %x (GDT entry)", ds);
    print_log("  SS    : %x (GDT entry)", ss);
    print_log("  IP    : %x", ip);
    print_log("  FLAG  : %x", eflag);
    if(cs%sizeof(struct GDTEntry)==0) {
        print_log("  PID   : %d", get_idt_reverse_pid_lookup_cs(cs));
        unsigned int abs_cs = get_gdt_baseaddress(gdt_table, GDT_TABLE_SIZE, cs/sizeof(struct GDTEntry));
        print_log("  abs IP: %x", abs_cs+ip);
        print_log("  abs CS: %x", abs_cs);
    } else {
        print_log("  PID   : invalid");
        print_log("  abs IP: invalid");
        print_log("  abs CS: invalid");
    }
    if(ds%sizeof(struct GDTEntry)==0) {
        print_log("  abs DS: %x", get_gdt_baseaddress(gdt_table, GDT_TABLE_SIZE, ds/sizeof(struct GDTEntry)));
    } else {
        print_log("  abs DS: invalid");
    }
    if(ss%sizeof(struct GDTEntry)==0) {
        print_log("  abs SS: %x", get_gdt_baseaddress(gdt_table, GDT_TABLE_SIZE, ss/sizeof(struct GDTEntry)));
    } else {
        print_log("  abs SS: invalid");
    }

    switch (id) {
        case 0x0D:
            PANIC(id, "[hw_exception] general_protection_fault");
    }
    PANIC(id, "[hw_exception] triggered: no handler");
}

void interrupt_handler_0x0D_general_protection_fault(int id, int ip, int cs) {
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
