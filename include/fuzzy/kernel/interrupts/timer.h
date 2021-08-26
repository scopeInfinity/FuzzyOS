#pragma once

void interrupt_pit_enable();

void interrupt_register_0x20_irq0_pit();

// return new user_sp
int create_infant_process_irq0_stack(int user_datasegment, int user_sp);
