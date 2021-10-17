#include <fuzzy/drivers/ps2/keyboard.h>
#include <fuzzy/drivers/ps2/ps2.h>

#include <drivers/keyboard/keyboard.h>

void ps2_keyboard_init() { keyboard_init(); }

char ps2_keyboard_get_key_pressed_blocking() {
    return keyboard_get_key_pressed_blocking();
}

char ps2_keyboard_get_key_pressed_poll() {
    return keyboard_get_key_pressed_poll();
}

int ps2_keyboard_get_kbhit() { return keyboard_get_kbhit(); }
