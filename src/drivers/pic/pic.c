#include <fuzzy/drivers/pic/pic.h>
#include <fuzzy/kernel/interrupts/interrupts.h>
#include <lib/utils/logging.h>

extern void _pic_init_low(int idt_irq0_pic1, int idt_irq0_pic2);
extern void _pic_pit_init();

extern void pic_timer_set_counter(unsigned short counter);
extern unsigned short pic_timer_get_counter();
extern void pic_pit_reset();

extern unsigned short _pic_readmask();
extern void _pic_writemask(unsigned short mask);

void pic_init() {
    print_log("PIC init.");
    _pic_init_low(IDT_IRQ_OFFSET, IDT_IRQ_OFFSET+8);

    // disable all IRQs
    _pic_writemask(0xFFFF);

    _pic_pit_init();
}

void pic_irq_enable(int irq) {
    int mask = _pic_readmask();
    mask = mask&(~(1<<irq));
    _pic_writemask(mask);
}

void pic_irq_disable(int irq) {
    int mask = _pic_readmask();
    mask = mask|((1<<irq));
    _pic_writemask(mask);
}