#pragma once

#include <stddef.h>

int graphics_switchto_320x200x256();
int graphics_write_320x200x256(int user_ds, uint8_t *__us_buffer);
