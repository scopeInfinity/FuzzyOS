#pragma once

void ps2_keyboard_init();

char ps2_keyboard_get_key_pressed_blocking();
char ps2_keyboard_get_key_pressed_poll();
int ps2_keyboard_get_kbhit();
