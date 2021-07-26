#include <fuzzy/kernel/interrupts.h>
#include <fuzzy/drivers/pic/pic.h>
#include <lib/utils/logging.h>

#define INT_MAX_VALUE 0x7FFFFFFF

extern void irq0_pit_handler_low();

// ticks counter
static unsigned int boot_ticks_0 = 0;
static unsigned int boot_ticks_1 = 0;

void timer_add_ticks(unsigned int inc) {
    int l0 = boot_ticks_0;
    boot_ticks_0 += inc;
    if(boot_ticks_0 < l0) {
        boot_ticks_1++;
    }
}

// absolute time
inline int get_time_since_boot_ms() {
    // This isn't accurate.
    // TODO: use boot_ticks_1
    int cycle_per_ms = PIC_PIT_FREQ/1000;
    int ms = boot_ticks_0/cycle_per_ms;
    return ms;
}

void irq0_pit_handler() {
    // called every X milli seconds.
    // time period is defined by pic_pit_set_counter.
    unsigned short ticks_jumped =  pic_pit_get_counter();

    int oldtime_ms = get_time_since_boot_ms();
    timer_add_ticks(ticks_jumped);
    int newtime_ms = get_time_since_boot_ms();
    if (oldtime_ms/1000 != newtime_ms/1000) {
        print_log(">> +1 second.");
    }
}

void interrupt_pit_enable() {
    pic_pit_set_counter(PIC_PIT_FREQ/100);  // 10ms
    pic_irq_enable(PIC_IRQ_PIT);
}

void interrupt_register_0x08_pit() {
    // As we are using BIOS default PIC mapping.
    populate_idt_entry_32bit(0x08, (unsigned int)irq0_pit_handler_low, 0, 0);
    pic_init();
}