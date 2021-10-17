#include <lib/ds/queue.h>

// Scan Code Set 2
// P in naming in prefix code
// RP in naming in additonal prefix code for release key
#define KEYBOARD_SC_RPF0_F1 0x05
#define KEYBOARD_SC_RPF0_F2 0x06
#define KEYBOARD_SC_RPF0_F3 0x04
#define KEYBOARD_SC_RPF0_F4 0x0C
#define KEYBOARD_SC_RPF0_F5 0x03
#define KEYBOARD_SC_RPF0_F6 0x0B
#define KEYBOARD_SC_RPF0_F7 0x83
#define KEYBOARD_SC_RPF0_F8 0x0A
#define KEYBOARD_SC_RPF0_F9 0x01
#define KEYBOARD_SC_RPF0_F10 0x09
#define KEYBOARD_SC_RPF0_F11 0x78
#define KEYBOARD_SC_RPF0_F12 0x07

#define KEYBOARD_SC_RPF0_back_tick 0x0E
#define KEYBOARD_SC_RPF0_1 0x16
#define KEYBOARD_SC_RPF0_2 0x1E
#define KEYBOARD_SC_RPF0_3 0x26
#define KEYBOARD_SC_RPF0_4 0x25
#define KEYBOARD_SC_RPF0_5 0x2E
#define KEYBOARD_SC_RPF0_6 0x36
#define KEYBOARD_SC_RPF0_7 0x3D
#define KEYBOARD_SC_RPF0_8 0x3E
#define KEYBOARD_SC_RPF0_9 0x46
#define KEYBOARD_SC_RPF0_0 0x45
#define KEYBOARD_SC_RPF0_hypen 0x4E
#define KEYBOARD_SC_RPF0_equals 0x55
#define KEYBOARD_SC_RPF0_backspace 0x66

#define KEYBOARD_SC_RPF0_A 0x1C
#define KEYBOARD_SC_RPF0_B 0x32
#define KEYBOARD_SC_RPF0_C 0x21
#define KEYBOARD_SC_RPF0_D 0x23
#define KEYBOARD_SC_RPF0_E 0x24
#define KEYBOARD_SC_RPF0_F 0x2B
#define KEYBOARD_SC_RPF0_G 0x34
#define KEYBOARD_SC_RPF0_H 0x33
#define KEYBOARD_SC_RPF0_I 0x43
#define KEYBOARD_SC_RPF0_J 0x3B
#define KEYBOARD_SC_RPF0_K 0x42
#define KEYBOARD_SC_RPF0_L 0x4B
#define KEYBOARD_SC_RPF0_M 0x3A
#define KEYBOARD_SC_RPF0_N 0x31
#define KEYBOARD_SC_RPF0_O 0x44
#define KEYBOARD_SC_RPF0_P 0x4D
#define KEYBOARD_SC_RPF0_Q 0x15
#define KEYBOARD_SC_RPF0_R 0x2D
#define KEYBOARD_SC_RPF0_S 0x1B
#define KEYBOARD_SC_RPF0_T 0x2C
#define KEYBOARD_SC_RPF0_U 0x3C
#define KEYBOARD_SC_RPF0_V 0x2A
#define KEYBOARD_SC_RPF0_W 0x1D
#define KEYBOARD_SC_RPF0_X 0x22
#define KEYBOARD_SC_RPF0_Y 0x35
#define KEYBOARD_SC_RPF0_Z 0x1A

#define KEYBOARD_SC_RPF0_keypad_0 0x70
#define KEYBOARD_SC_RPF0_keypad_1 0x69
#define KEYBOARD_SC_RPF0_keypad_2 0x72
#define KEYBOARD_SC_RPF0_keypad_3 0x7A
#define KEYBOARD_SC_RPF0_keypad_4 0x6B
#define KEYBOARD_SC_RPF0_keypad_5 0x73
#define KEYBOARD_SC_RPF0_keypad_6 0x74
#define KEYBOARD_SC_RPF0_keypad_7 0x6C
#define KEYBOARD_SC_RPF0_keypad_8 0x75
#define KEYBOARD_SC_RPF0_keypad_9 0x7D
#define KEYBOARD_SC_RPF0_keypad_dot 0x71
#define KEYBOARD_SC_RPF0_keypad_minus 0x7B
#define KEYBOARD_SC_RPF0_keypad_mul 0x7C
#define KEYBOARD_SC_RPF0_keypad_plus 0x79
#define KEYBOARD_SC_PE0_RPF0_keypad_enter 0x5A

#define KEYBOARD_SC_RPF0_tab 0x0D
#define KEYBOARD_SC_RPF0_caps_lock 0x58
#define KEYBOARD_SC_RPF0_left_shift 0x12
#define KEYBOARD_SC_RPF0_left_control 0x14
#define KEYBOARD_SC_RPF0_left_alt 0x11

#define KEYBOARD_SC_RPF0_space 0x29

#define KEYBOARD_SC_RPF0_right_shift 0x59
#define KEYBOARD_SC_RPF0_enter 0x5A
#define KEYBOARD_SC_PE0_RPF0_right_control 0x14
#define KEYBOARD_SC_PE0_RPF0_right_alt 0x11

#define KEYBOARD_SC_RPF0_blackslash 0x5D
#define KEYBOARD_SC_RPF0_brace_sq_o 0x54
#define KEYBOARD_SC_RPF0_brace_sq_c 0x5B
#define KEYBOARD_SC_RPF0_semicolon 0x4C
#define KEYBOARD_SC_RPF0_quotation 0x52
#define KEYBOARD_SC_RPF0_comma 0x41
#define KEYBOARD_SC_RPF0_dot 0x49
#define KEYBOARD_SC_RPF0_escape 0x76
#define KEYBOARD_SC_RPF0_slash 0x4A

#define KEYBOARD_SC_RPF0_NumberLock 0x77
#define KEYBOARD_SC_RPF0_scroll_lock 0x7E

#define KEYBOARD_SC_PE0_RPF0_delete 0x71
#define KEYBOARD_SC_PE0_RPF0_end 0x69
#define KEYBOARD_SC_PE0_RPF0_home 0x6C
#define KEYBOARD_SC_PE0_RPF0_insert 0x70
#define KEYBOARD_SC_PE0_RPF0_page_down 0x7A
#define KEYBOARD_SC_PE0_RPF0_page_up 0x7D

#define KEYBOARD_SC_PE0_RPF0_cursor_left 0x6B
#define KEYBOARD_SC_PE0_RPF0_cursor_down 0x72
#define KEYBOARD_SC_PE0_RPF0_cursor_up 0x75
#define KEYBOARD_SC_PE0_RPF0_cursor_right 0x74

#define KEYBOARD_SC_PE0_RPF0_volume_down 0x21
#define KEYBOARD_SC_PE0_RPF0_volume_up 0x32

static const unsigned char KEYBOARD_ASCII_MAPH_AZ[] = {
    KEYBOARD_SC_RPF0_A, KEYBOARD_SC_RPF0_B, KEYBOARD_SC_RPF0_C,
    KEYBOARD_SC_RPF0_D, KEYBOARD_SC_RPF0_E, KEYBOARD_SC_RPF0_F,
    KEYBOARD_SC_RPF0_G, KEYBOARD_SC_RPF0_H, KEYBOARD_SC_RPF0_I,
    KEYBOARD_SC_RPF0_J, KEYBOARD_SC_RPF0_K, KEYBOARD_SC_RPF0_L,
    KEYBOARD_SC_RPF0_M, KEYBOARD_SC_RPF0_N, KEYBOARD_SC_RPF0_O,
    KEYBOARD_SC_RPF0_P, KEYBOARD_SC_RPF0_Q, KEYBOARD_SC_RPF0_R,
    KEYBOARD_SC_RPF0_S, KEYBOARD_SC_RPF0_T, KEYBOARD_SC_RPF0_U,
    KEYBOARD_SC_RPF0_V, KEYBOARD_SC_RPF0_W, KEYBOARD_SC_RPF0_X,
    KEYBOARD_SC_RPF0_Y, KEYBOARD_SC_RPF0_Z};

static const unsigned char KEYBOARD_ASCII_MAPH_09[] = {
    KEYBOARD_SC_RPF0_0, KEYBOARD_SC_RPF0_1, KEYBOARD_SC_RPF0_2,
    KEYBOARD_SC_RPF0_3, KEYBOARD_SC_RPF0_4, KEYBOARD_SC_RPF0_5,
    KEYBOARD_SC_RPF0_6, KEYBOARD_SC_RPF0_7, KEYBOARD_SC_RPF0_8,
    KEYBOARD_SC_RPF0_9};

static const unsigned char KEYBOARD_ASCII_MAPH_KEYPAD_09[] = {
    KEYBOARD_SC_RPF0_keypad_0, KEYBOARD_SC_RPF0_keypad_1,
    KEYBOARD_SC_RPF0_keypad_2, KEYBOARD_SC_RPF0_keypad_3,
    KEYBOARD_SC_RPF0_keypad_4, KEYBOARD_SC_RPF0_keypad_5,
    KEYBOARD_SC_RPF0_keypad_6, KEYBOARD_SC_RPF0_keypad_7,
    KEYBOARD_SC_RPF0_keypad_8, KEYBOARD_SC_RPF0_keypad_9};

// Key ScanCode and ASCII code are listed alternatively in the next list.
static const unsigned char KEYBOARD_ASCII_MAPH_OTHERS[] = {
    KEYBOARD_SC_RPF0_space,
    ' ',
    KEYBOARD_SC_RPF0_comma,
    ',',
    KEYBOARD_SC_RPF0_dot,
    '.',
    KEYBOARD_SC_RPF0_slash,
    '/',
    KEYBOARD_SC_RPF0_semicolon,
    ';',
    KEYBOARD_SC_RPF0_quotation,
    '\'',
    KEYBOARD_SC_RPF0_brace_sq_o,
    '[',
    KEYBOARD_SC_RPF0_brace_sq_c,
    ']',
    KEYBOARD_SC_RPF0_blackslash,
    '\\',

    KEYBOARD_SC_RPF0_hypen,
    '-',
    KEYBOARD_SC_RPF0_equals,
    '=',

    KEYBOARD_SC_RPF0_enter,
    '\n',
    KEYBOARD_SC_RPF0_keypad_dot,
    '.',
    KEYBOARD_SC_RPF0_keypad_minus,
    '-',
    KEYBOARD_SC_RPF0_keypad_mul,
    '*',
    KEYBOARD_SC_RPF0_keypad_plus,
    '+',
    KEYBOARD_SC_PE0_RPF0_keypad_enter,
    '\n',
    KEYBOARD_SC_RPF0_backspace,
    '\b'};

static unsigned char KEYBOARD_ASCII_MAPPING[256];
extern int keyboard_buffer[];
static char last_ascii;
static int last_ascii_available = 0;
static char tmp_juggad_is_f0 = 0;

void keyboard_scanner_ascii_clear() { queue_clear(keyboard_buffer); }

int keyboard_scanner_ascii_is_available() { return last_ascii_available; }

char keyboard_scanner_ascii_get() {
    char c = last_ascii;
    last_ascii_available = 0;
    last_ascii = 0;
    return c;
}

void keyboard_scanner_handler_init() {
    for (int i = 0; i < sizeof(KEYBOARD_ASCII_MAPPING); ++i) {
        KEYBOARD_ASCII_MAPPING[i] = 0;
    }
    for (int i = 0; i < sizeof(KEYBOARD_ASCII_MAPH_AZ); i++) {
        unsigned char code = KEYBOARD_ASCII_MAPH_AZ[i];
        unsigned char c = 'a' + i;
        KEYBOARD_ASCII_MAPPING[code] = c;
    }
    for (int i = 0; i < sizeof(KEYBOARD_ASCII_MAPH_09); i++) {
        unsigned char code = KEYBOARD_ASCII_MAPH_09[i];
        unsigned char c = '0' + i;
        KEYBOARD_ASCII_MAPPING[code] = c;
    }
    for (int i = 0; i < sizeof(KEYBOARD_ASCII_MAPH_KEYPAD_09); i++) {
        unsigned char code = KEYBOARD_ASCII_MAPH_KEYPAD_09[i];
        unsigned char c = '0' + i;
        KEYBOARD_ASCII_MAPPING[code] = c;
    }
    for (int i = 0; i < sizeof(KEYBOARD_ASCII_MAPH_OTHERS); i += 2) {
        unsigned char code = KEYBOARD_ASCII_MAPH_OTHERS[i];
        unsigned char c = KEYBOARD_ASCII_MAPH_OTHERS[i + 1];
        KEYBOARD_ASCII_MAPPING[code] = c;
    }
}

int keyboard_scanner_handle_buffer() {
    // For now just taking care of some of the key press and disgarding others.
    int len = queue_size(keyboard_buffer);
    if (len == 0)
        return 0;
    int c0 = 0;
    c0 = queue_front(keyboard_buffer);
    queue_pop(keyboard_buffer);

    // Discard keyboard response for key released.
    if (tmp_juggad_is_f0) {
        tmp_juggad_is_f0 = 0;
        return 1;
    }
    if (c0 == 0xF0) {
        tmp_juggad_is_f0 = 1;
        return 1;
    }

    if (KEYBOARD_ASCII_MAPPING[c0] > 0) {
        last_ascii = KEYBOARD_ASCII_MAPPING[c0];
        last_ascii_available = 1;
    }
    if (last_ascii_available)
        return 1;
    return 0;
}
