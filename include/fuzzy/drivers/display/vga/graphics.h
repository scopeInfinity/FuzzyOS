#pragma once

#include <stddef.h>

int graphics_set_mode(uint8_t video_mode);
uint8_t graphics_get_mode();

int graphics_write_320x200x256(int user_ds, uint8_t *__us_buffer);
