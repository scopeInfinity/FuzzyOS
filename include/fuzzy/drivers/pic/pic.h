#pragma once

#include <stddef.h>

#define PIC_PIT_FREQ        1193182  // hz
#define PIC_PIT_MAX_COUNTER 0xFFFF

#define PIC_IRQ_PIT 0

void pic_init();

void pic_writemask_new(uint16_t mask);
uint16_t pic_readmask_new();

void pic_irq_enable(int irq);
void pic_irq_disable(int irq);
