#pragma once

void interrupt_pit_enable();

void interrupt_register_0x20_irq0_pit();

int create_infant_process_irq0_stack(int ds_ss_es_fs);