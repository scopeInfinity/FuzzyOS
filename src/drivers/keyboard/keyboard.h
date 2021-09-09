#pragma once

void keyboard_init();

char keyboard_get_key_pressed_blocking();
char keyboard_get_key_pressed_poll();
int keyboard_get_kbhit();
