#include <fuzzy/drivers/port.h>
#include <fuzzy/drivers/pic/pic.h>
#include <fuzzy/kernel/interrupts/interrupts.h>
#include <lib/utils/logging.h>

void pic_init() {
    print_log("[driver][pic] init");
    int idt_irq0_pic1 = IDT_IRQ_OFFSET;
    int idt_irq0_pic2 = IDT_IRQ_OFFSET+8;

    outb(PORT_PIC1_CMD, 0x11);
    outb(PORT_PIC2_CMD, 0x11);

    // Remap IRQ
    // FuzzyOS won't be using IRQ0 in real mode.
    outb(PORT_PIC1_DATA, idt_irq0_pic1);
    outb(PORT_PIC2_DATA, idt_irq0_pic2);

    outb(PORT_PIC1_DATA, 4);
    outb(PORT_PIC2_DATA, 2);

    outb(PORT_PIC1_DATA, 1);
    outb(PORT_PIC2_DATA, 1);

    // disable all IRQs
    pic_writemask_new(0xFFFF);

    pit_init();
}

void pic_writemask_new(uint16_t mask) {
    outb(PORT_PIC1_DATA, mask&0xFF);
    outb(PORT_PIC2_DATA, mask>>8);
}

uint16_t pic_readmask_new() {
    uint16_t mask = inputb(PORT_PIC1_DATA);
    mask |= inputb(PORT_PIC2_DATA) << 8;
    return mask;
}

void pic_irq_enable(int irq) {
    int mask = pic_readmask_new();
    mask = mask&(~(1<<irq));
    pic_writemask_new(mask);
}

void pic_irq_disable(int irq) {
    int mask = pic_readmask_new();
    mask = mask|((1<<irq));
    pic_writemask_new(mask);
}
