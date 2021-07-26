#pragma once

#define PIC_PIT_FREQ        1193182  // hz
#define PIC_PIT_MAX_COUNTER 0xFFFF

#define PIC_IRQ_PIT 0

void interrupt_register_0x08_pit();
void interrupt_pit_enable();

void pic_irq_enable(int irq);
void pic_irq_disable(int irq);

void pic_pit_set_counter(unsigned short counter);
unsigned short pic_pit_get_counter();
void pic_pit_reset();
