#include <drivers/keyboard/keyboard.h>
#include <lib/ds/queue.h>
#include <lib/utils/output.h>
#include <lib/utils/time.h>
#include <lib/utils/panic.h>

#include "scancode_handler.c"

#define DRIVERS_KEYBOARD_PORT_DATA      0x60
#define DRIVERS_KEYBOARD_PORT_STATUS    0x64
#define DRIVERS_KEYBOARD_PORT_COMMAND   0x64


#define STATUS_OUTPUT_BUFFER 0x1
#define STATUS_INPUT_BUFFER  0x2
#define STATUS_SYSTEM_FLAG 0x4
#define STATUS_CMD_DATA 0x8 // 0:cmd 1:data

// Using USB Legacy Support
extern void port_write(unsigned short port, unsigned char value);
extern unsigned char port_read(unsigned short port);
void __stack_chk_fail(void) {
    PANIC(0, "Kernel stack overflow!!!");
}

void print_status(const char *message, int status) {
    move_xy(0, TEXT_WINDOW_HEIGHT-1);
    print_line(message);
    print_int(status);
    int left = TEXT_WINDOW_WIDTH - get_display_text_x();
    while(left>0) {
        print_char(' ');
        left--;
    }
}

void sleep_mini(int s) {
    while(s>0) {
        s--;
    }
}

void keyboard_wait() {
    sleep_mini(100000);
}

const int WAIT_FOR_STATUS_TIMEOUT = 1000000;
const int WAIT_FOR_STATUS_KEYSCANCODE_TIMEOUT = 10;

int wait_for_status_flag_timeout_low(unsigned char flag, unsigned char check_true, int timeout, int canpanic) {
    while(timeout>0) {
        if((((port_read(DRIVERS_KEYBOARD_PORT_STATUS))&flag)!=0)^check_true) {
             // waiting
        } else {
            // poll done
            return 1;
        }
        timeout--;
    }
    if(!canpanic) return 0;
    if(check_true) {
        PANIC(flag,"TIMEOUT: status bit set");
    } else {
        PANIC(flag,"TIMEOUT: status bit reset");
    }
}
int wait_for_status_flag_timeout(unsigned char flag, unsigned char check_true, int timeout) {
    return wait_for_status_flag_timeout_low(flag, check_true, timeout, 1);
}
int wait_for_status_flag_nopanic(unsigned char flag, unsigned char check_true) {
    return wait_for_status_flag_timeout_low(flag, check_true, WAIT_FOR_STATUS_TIMEOUT, 0);
}
int wait_for_status_flag_nopanic_timeout(unsigned char flag, unsigned char check_true, int timeout) {
    return wait_for_status_flag_timeout_low(flag, check_true, timeout, 0);
}
int wait_for_status_flag(unsigned char flag, unsigned char check_true) {
    return wait_for_status_flag_timeout_low(flag, check_true, WAIT_FOR_STATUS_TIMEOUT, 1);
}

unsigned char ps2_controller_send_command(unsigned char cmd, int want_reply) {
    unsigned char out;
    port_write(DRIVERS_KEYBOARD_PORT_COMMAND, cmd);
    keyboard_wait();
    if(!want_reply) {
        return 0;
    }
    wait_for_status_flag(STATUS_OUTPUT_BUFFER, 1);
    unsigned char data = port_read(DRIVERS_KEYBOARD_PORT_DATA);
    return data;
}

unsigned char ps2_controller_send_command_with_data(unsigned char cmd, unsigned char data, int want_reply) {
    unsigned char out;
    port_write(DRIVERS_KEYBOARD_PORT_COMMAND, cmd);
    wait_for_status_flag(STATUS_INPUT_BUFFER, 0);
    port_write(DRIVERS_KEYBOARD_PORT_DATA, data);
    keyboard_wait();
    if(!want_reply) {
        return 0;
    }
    wait_for_status_flag(STATUS_OUTPUT_BUFFER, 1);
    data = port_read(DRIVERS_KEYBOARD_PORT_DATA);
    return data;
}

unsigned char write_to_ps2_first_port(unsigned char byte, int want_reply) {
    unsigned char out;
    wait_for_status_flag_timeout(STATUS_INPUT_BUFFER, 0, WAIT_FOR_STATUS_TIMEOUT);
    port_write(DRIVERS_KEYBOARD_PORT_DATA, byte);
    keyboard_wait();
    if(!want_reply) {
        return 0;
    }
    wait_for_status_flag_timeout(STATUS_OUTPUT_BUFFER, 1, WAIT_FOR_STATUS_TIMEOUT);
    out = port_read(DRIVERS_KEYBOARD_PORT_DATA);
    return out;
}

unsigned char write_to_ps2_second_port(unsigned char byte, int want_reply) {
    unsigned char out;
    port_write(DRIVERS_KEYBOARD_PORT_COMMAND, 0xD4);
    wait_for_status_flag_timeout(STATUS_INPUT_BUFFER, 0, WAIT_FOR_STATUS_TIMEOUT);
    port_write(DRIVERS_KEYBOARD_PORT_DATA, byte);
    keyboard_wait();
    if(!want_reply) {
        return 0;
    }
    wait_for_status_flag_timeout(STATUS_OUTPUT_BUFFER, 1, WAIT_FOR_STATUS_TIMEOUT);
    out = port_read(DRIVERS_KEYBOARD_PORT_DATA);
}

unsigned char read_data_reply() {
    keyboard_wait();
    wait_for_status_flag_timeout(STATUS_OUTPUT_BUFFER, 1, WAIT_FOR_STATUS_TIMEOUT);
    return port_read(DRIVERS_KEYBOARD_PORT_DATA);
}

#define KEYBOARD_BUFFER_SIZE 64
int keyboard_buffer[KEYBOARD_BUFFER_SIZE+3];

void keyboard_scanner_handler_init();
void keyboard_scanner_ascii_clear();
int keyboard_scanner_handle_buffer();

void keyboard_scanner_init() {
    ASSERT( queue_init(keyboard_buffer, KEYBOARD_BUFFER_SIZE) );
    keyboard_scanner_handler_init();
}

int keyboard_scanner_step() {
    int state_change = 0;
    int qc = queue_capacity(keyboard_buffer);
    int qs = queue_size(keyboard_buffer);
    if(qs<qc) {
        int got_response = wait_for_status_flag_nopanic_timeout(STATUS_OUTPUT_BUFFER, 1, WAIT_FOR_STATUS_KEYSCANCODE_TIMEOUT);
        if(!got_response) return 0;
        unsigned char out = read_data_reply();
        if(out == 0) return 0;
        state_change = 1;

        queue_push(keyboard_buffer, out);
        qs++;
    }
    keyboard_scanner_handle_buffer();
    return state_change;
}

char keyboard_get_key_pressed_blocking() {
    keyboard_scanner_ascii_clear();
    while(!keyboard_scanner_ascii_is_available()) {
        keyboard_scanner_step();
    }
    char c = keyboard_scanner_ascii_get();
    return c;
}

void keyboard_init() {
    unsigned char original_colors =  get_color_fgbg();

    set_color_bg(C_LIGHT_GRAY);
    set_color_fg(C_BLACK);

    sleep_mini(3000000);

    unsigned char out;
    // disable PS/2 first port
    ps2_controller_send_command(0XAD, 0);
    // disable PS/2 second port
    ps2_controller_send_command(0XA7, 0);
    // flush output buffer
    wait_for_status_flag(STATUS_OUTPUT_BUFFER, 0);

    out = ps2_controller_send_command(0X20, 1);
    out = out&(~0b1000011);
    int dual_ps2_controller = 0;
    if (out & (0b10000)) {
        dual_ps2_controller = 1;
    }
    print_status("Maybe dual PS2 Controller", dual_ps2_controller);
    ps2_controller_send_command_with_data(0x60, out, 0);


    out = ps2_controller_send_command(0XAA, 1);
    if (out != 0x55) {
        PANIC(out, "PS2 controller self test failed.");
    }

    if(dual_ps2_controller) {
        ps2_controller_send_command(0XA8, 0);
        out = ps2_controller_send_command(0X20, 1);
        if (!(out & (0b10000))) {
            dual_ps2_controller = 0;
            print_status("Enabling PS2 port failed, thus disabling dual_ps2_controller", 0);
        } else {
            // disabling second port again.
            ps2_controller_send_command(0xA7, 0);
        }
    }

    out = ps2_controller_send_command(0XAB, 1);
    if (out != 0x00) {
        PANIC(out, "PS2 first port test failed.");
    }
    if(dual_ps2_controller) {
        out = ps2_controller_send_command(0XA9, 1);
        if (out != 0x00) {
            PANIC(out, "PS2 second port test failed.");
        }
    }

    print_status("Dual PS2 Controller: ", dual_ps2_controller);

    // enable first port
    ps2_controller_send_command(0xAE, 0);
    if(dual_ps2_controller) {
        // enable second port
        ps2_controller_send_command(0xA8, 0);
    }

    // enable interrupts
    out = ps2_controller_send_command(0X20, 1);
    out |= 0b11;
    ps2_controller_send_command_with_data(0x60, out, 0);


    // reset device
    out = write_to_ps2_first_port(0xFF, 1);
    if (out != 0xFA) {
        PANIC(out, "reset ps/2 first failed");
    }
    out = read_data_reply();
    if (out != 0xAA) {
        PANIC(out, "reset ps/2 first port, keyboard self test failed");
    }

    if(dual_ps2_controller) {
        // reset second port
        out = write_to_ps2_second_port(0xFF, 1);
        if (out != 0xFA) {
            PANIC(out, "reset ps/2 second failed");
        }
    }

    // detect device type
    out = write_to_ps2_first_port(0xF5, 1);
    if (out != 0xFA) {
        PANIC(out, "disable scanning failed.");
    }
    out = write_to_ps2_first_port(0xF2, 1);
    if (out != 0xFA) {
        PANIC(out, "device identity failed.");
    }
    unsigned char dev_d0 = read_data_reply();
    unsigned char dev_d1 = read_data_reply();

    // Testing for device 0xAB 0x83: MF2 keyboard
    if(dev_d0 != 0xAB) {
        PANIC(dev_d0, "testing for device id 0xAB 0x83, found xx 0x83.");
    }
    if(dev_d1 != 0x83) {
        PANIC(dev_d1, "testing for device id 0xAB 0x83, found 0xAB xx.");
    }

    // Caps
    write_to_ps2_first_port(0xED, 0);
    out = write_to_ps2_first_port(7, 1);
    if (out != 0xFA) {
        PANIC(out, "caps failed");
    }
    // get scan code
    write_to_ps2_first_port(0xF0, 0);
    out = write_to_ps2_first_port(0, 1);
    if (out != 0xFA) {
        PANIC(out, "failed to get scan code");
    } else {
        // Not yet working
        // out = read_data_reply();
        // PANIC(out, "got get scan code");
    }

    // set scan code
    write_to_ps2_first_port(0xF0, 0);
    out = write_to_ps2_first_port(2, 1);
    if (out != 0xFA) {
        PANIC(out, "failed to set scan code");
    }

    // enable scanning
    out = write_to_ps2_first_port(0xF4, 1);
    if (out != 0xFA) {
        PANIC(out, "scan failed");
    }
    keyboard_scanner_init();
    set_color_fgbg(original_colors);
}