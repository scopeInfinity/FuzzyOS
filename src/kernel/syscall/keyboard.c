#include <fuzzy/kernel/syscall/keyboard.h>

#include <conio.h>

static int _keyboard_getch() {
    return keyboard_get_key_pressed_poll();
}

static int _keyboard_kbhit() {
    return keyboard_get_kbhit();
}

int syscall_0_keyboard(int operation,int a1,int a2,int a3, int user_ds) {
    switch (operation) {
        case SYSCALL_KEYBOARD_SUB_GETCH:
            return _keyboard_getch();
        case SYSCALL_KEYBOARD_SUB_KBHIT:
            return _keyboard_kbhit();
    }
    return -1;
}