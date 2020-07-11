// UnderDevelopment: Not in functional state.
// It's NOT Working :(...
// Also keyboard is depending lib/utils which depends on drivers :(
#include <drivers/keyboard/keyboard.h>
#include <lib/utils/io.h>
#include <lib/utils/time.h>
#include <lib/utils/panic.h>

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

void print_status(const char *message, int status) {
    set_color_bg(C_WHITE);
    set_color_fg(C_BLACK);
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
void wait_for_status_flag(unsigned char flag, unsigned char check_true) {
    while((((port_read(DRIVERS_KEYBOARD_PORT_STATUS))&flag)!=0)^check_true);
}
void wait_for_status_flag_timeout(unsigned char flag, unsigned char check_true, int timeout) {
    while(timeout>0) {
        if((((port_read(DRIVERS_KEYBOARD_PORT_STATUS))&flag)!=0)^check_true) {
             // waiting
        } else {
            // poll done
            return;
        }
        timeout--;
    }
    if(check_true) {
        PANIC(flag,"TIMEOUT: status bit set");
    } else {
        PANIC(flag,"TIMEOUT: status bit reset");
    }
}

unsigned char send_command(unsigned char cmd, int want_reply) {
    unsigned char out;
    sleep_mini(100000);
    port_write(DRIVERS_KEYBOARD_PORT_COMMAND, cmd);
    sleep_mini(100000);
    if(!want_reply) {
        return 0;
    }
    wait_for_status_flag(STATUS_OUTPUT_BUFFER, 1);
    // wait_for_status_flag(STATUS_INPUT_BUFFER, 0);
    sleep_mini(100000);
    unsigned char data = port_read(DRIVERS_KEYBOARD_PORT_DATA);
    return data;
}

unsigned char send_command_with_data(unsigned char cmd, unsigned char data, int want_reply) {
    unsigned char out;

    port_write(DRIVERS_KEYBOARD_PORT_COMMAND, cmd);
    sleep_mini(1000000);
    wait_for_status_flag(STATUS_INPUT_BUFFER, 0);
    port_write(DRIVERS_KEYBOARD_PORT_DATA, data);
    sleep_mini(1000000);
    if(!want_reply) {
        return 0;
    }
    sleep_mini(1000000);
    wait_for_status_flag(STATUS_OUTPUT_BUFFER, 1);
    data = port_read(DRIVERS_KEYBOARD_PORT_DATA);
    return data;
}
unsigned char read_reply() {
    sleep_mini(1000000);
    wait_for_status_flag_timeout(STATUS_OUTPUT_BUFFER, 1, 1000000);
    return port_read(DRIVERS_KEYBOARD_PORT_DATA);
}

unsigned char write_to_ps2_first_port(unsigned char byte, int want_reply) {
    unsigned char out;
    for(int i=0;i<3;i++) {
        wait_for_status_flag_timeout(STATUS_INPUT_BUFFER, 0, 1000000);
        port_write(DRIVERS_KEYBOARD_PORT_DATA, byte);
        sleep_mini(1000000);
        if(!want_reply) {
            return 0;
        }
        wait_for_status_flag_timeout(STATUS_OUTPUT_BUFFER, 1, 1000000);
        out = port_read(DRIVERS_KEYBOARD_PORT_DATA);
        if(out!=0xFE) {
            break;
        }
    }
    return out;
}
void write_to_ps2_second_port(unsigned char byte) {
    port_write(DRIVERS_KEYBOARD_PORT_COMMAND, 0x64);
    sleep_mini(1000000);
    wait_for_status_flag_timeout(STATUS_INPUT_BUFFER, 0, 1000000);
    port_write(DRIVERS_KEYBOARD_PORT_DATA, byte);
    sleep_mini(1000000);
}

void keyboard_init() {
    sleep_mini(30000000);
    unsigned char out;

    // disable PS/2 first port
    send_command(0XAD, 0);
    // disable PS/2 second port
    send_command(0XA7, 0);
    // flush output buffer
    wait_for_status_flag(STATUS_OUTPUT_BUFFER, 0);

    out = send_command(0X20, 1);
    out = out&(~0b1000011);
    int second_port_exists = 0;
    if (out & (0b10000)) {
        second_port_exists = 1;
    }
    print_status("PS2 Port Count is ", 1+second_port_exists);
    send_command_with_data(0x60, out, 0);
    out = send_command(0XAA, 1);
    if (out != 0x55) {
        PANIC(out, "PS2 self test failed.");
    }

    if(second_port_exists) {
        send_command(0XA8, 0);
        out = send_command(0X20, 1);
        if (!(out & (0b10000))) {
            // should be clear
            second_port_exists = 0;
            send_command(0xA7, 0);
            print_status("PS2 second port disabled DNWAIS.", 0);
        } else {
            // can't have dual channel
            print_status("PS2 can't have dual channel.", 0);
        }
    }
    out = send_command(0XAB, 1);
    if (out != 0x00) {
        PANIC(out, "PS2 first port test failed.");
    }
    if(second_port_exists) {
        out = send_command(0XA9, 1);
        if (out != 0x00) {
            PANIC(out, "PS2 second port test failed.");
        }
    }
    // enable first port
    write_to_ps2_first_port(0xAE, 0);
    if(second_port_exists) {
        // enable second port
        write_to_ps2_second_port(0xA8);
    }
    // reset device
    out = write_to_ps2_first_port(0xFF, 1);
    if (out != 0xFA) {
        PANIC(out, "reset ps/2 first failed");
    }
    // if(second_port_exists) {
    //     // reset second port
    //     write_to_ps2_first_port(0xFF);
    //     out = read_reply();
    //     if (out != 0xFA) {
    //         PANIC(out, "reset ps/2 second failed");
    //     }
    // }

    // scan code
    // write_to_ps2_first_port(0xF0, 0);
    // out = write_to_ps2_first_port(0, 1);
    // if (out != 0xFA) {
    //     PANIC(out, "failed to get scan code");
    // } else {
    //     PANIC(out, "got get scan code");
    // }
    // write_to_ps2_first_port(0xF4);

    // Caps
    write_to_ps2_first_port(0xED, 0);
    out = write_to_ps2_first_port(7, 1);
    if (out != 0xFA) {
        PANIC(out, "caps failed");
    }
    // write_to_ps2_first_port(0xF4);
    // out = read_reply();
    // if (out != 0xFA) {
    //     PANIC(out, "scan failed");
    // }

    // // detect device type
    out = write_to_ps2_first_port(0xF5, 1);
    if (out != 0xFA) {
        PANIC(out, "disable scanning failed.");
    }
    out = write_to_ps2_first_port(0xF2, 1);
    if (out != 0xFA) {
        PANIC(out, "device identity failed.");
    }
    // 0xab
    unsigned char dev_d0 = read_reply();
    unsigned char dev_d1 = read_reply();
    PANIC(dev_d0, "device id");

    PANIC(1, "end");
}