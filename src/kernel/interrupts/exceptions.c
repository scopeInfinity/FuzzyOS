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

#define gdt_address_from_segment(s, gdte_size) (((s)%(gdte_size)==0)?get_gdt_baseaddress(gdt_table, GDT_TABLE_SIZE, (s)/(gdte_size)):-1)

struct exceptionContext {
    int id;  // exception id

    int ss, ds, es, fs, gs;
    int edi, esi, ebp, esp, ebx, edx, ecx, eax;

    // at end
    int err_code;  // can be optional for some interrupts.
    int ip, cs, eflag;
};

void interrupt_handler_0x00_0x1F_exception(struct exceptionContext context) {
    // written for handler with error code otherwise ip, cs and eflag will skew.
    const int gdte_size = sizeof(struct GDTEntry);
    panic_screen_init();

    print_log("Hardware exception %d (0x%x) triggered", context.id, context.id);
    print_log("  Error Code: %x", context.err_code);
    if(context.cs%gdte_size==0) {
        print_log("  PID   : %d", get_idt_reverse_pid_lookup_cs(context.cs));
    }
    print_log("  FLAG  : %x", context.eflag);
    print_log("  IP    : %x", context.ip);
    print_log("  CS    : %x => %x", context.cs, gdt_address_from_segment(context.cs, gdte_size));
    print_log("  DS    : %x => %x", context.ds, gdt_address_from_segment(context.ds, gdte_size));
    print_log("  SS    : %x => %x", context.ss, gdt_address_from_segment(context.ss, gdte_size));
    print_log("  ES    : %x => %x", context.es, gdt_address_from_segment(context.es, gdte_size));
    print_log("  FS    : %x => %x", context.fs, gdt_address_from_segment(context.fs, gdte_size));
    print_log("  GS    : %x => %x", context.gs, gdt_address_from_segment(context.gs, gdte_size));
    print_log("  EAX: %x EBX: %x ECX: %x EDX: %x", context.eax, context.ebx, context.ecx, context.edx);
    print_log("  ESP: %x EBP: %x ESI: %x EDI: %x", context.esp, context.ebp, context.esi, context.edi);

    // stack trace
    {
        const int stack_trace_max_depth = 5;
        int last_ebp = context.ebp;
        // stack[ebp] => previous_ebp, if previous_ebp == 0 then break;
        for(int i=0; i<stack_trace_max_depth; i++) {
            // to be implemented
            break;
        }

    }


    switch (context.id) {
        case 0x0D:
            PANIC(context.id, "[hw_exception] general_protection_fault");
    }
    PANIC(context.id, "[hw_exception] triggered: no handler");
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
