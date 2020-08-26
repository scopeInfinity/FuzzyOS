#pragma once

#define PIC_PIT_FREQ        1193182  // hz
#define PIC_PIT_MAX_COUNTER 0xFFFF

#define PIC_IRQ_PIT 0

void pic_init();

void pic_timer_configure();
void pic_end_of_interrupt();

void pic_timer_set_counter(unsigned short counter);
void pic_timer_reload_counter();

void pic_irq_enable(int irq);
void pic_irq_disable(int irq);
