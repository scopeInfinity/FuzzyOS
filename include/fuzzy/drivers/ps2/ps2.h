#pragma once

#include <stddef.h>

void ps2_init();

uint8_t ps2_read_data();
void ps2_write_port1(uint8_t byte);
void ps2_controller_wait_for_empty_input();
void ps2_controller_wait_for_full_output();

void interrupt_register_0x21_0x2C_irq1_ir12_keyboard_mouse();
void irq1_handler();
void irq12_handler();

