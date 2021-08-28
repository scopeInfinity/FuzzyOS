#pragma once

#include <stddef.h>

static uint16_t frequency_dividor;

void pit_init();

void pit_set_counter(uint16_t counter);
uint16_t pit_get_counter();

void pit_reload_counter();
void pit_reset();
