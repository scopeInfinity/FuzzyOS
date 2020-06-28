#ifndef __KERNEL_DRIVERS_KEYBOARD
#define __KERNEL_DRIVERS_KEYBOARD

#include <kernel/essentials.c>

#define DRIVERS_KEYBOARD_PORT_DATA      0x60
#define DRIVERS_KEYBOARD_PORT_STATUS    0x64
#define DRIVERS_KEYBOARD_PORT_COMMAND   0x64

// Using USB Legacy Support
extern void port_write(unsigned short port, unsigned char value);
extern unsigned char port_read(unsigned short port);

void keyboard_init() {
    kernel_status("Loading Keyboard Drivers...", 0);
    unsigned char out;
    port_write(DRIVERS_KEYBOARD_PORT_COMMAND, 0xAD);
    port_write(DRIVERS_KEYBOARD_PORT_COMMAND, 0xA7);
    while(1) {
        // clear the buffer
        out = port_read(DRIVERS_KEYBOARD_PORT_STATUS);
        if(!(out&1)) break;
    }
    port_write(DRIVERS_KEYBOARD_PORT_COMMAND, 0x20);
    port_write(DRIVERS_KEYBOARD_PORT_COMMAND, 0x60);
    out = port_read(DRIVERS_KEYBOARD_PORT_STATUS);
    int second_port_exists = 0;
    if(out&(0b10000)) {
        second_port_exists = 1;
        kernel_status("Loading Keyboard Drivers: Second Port Exists", 0);
    }
    // clear 0, 1 and 6.
    // test bit 5
    port_write(DRIVERS_KEYBOARD_PORT_COMMAND, 0xAA);
    out = port_read(DRIVERS_KEYBOARD_PORT_STATUS);
    if (out != 0x55) {
        PANIC(out, "PS2 self test failed.");
    }
    // enable PS2 first port.
    port_write(DRIVERS_KEYBOARD_PORT_COMMAND, 0xAE);
    port_write(DRIVERS_KEYBOARD_PORT_COMMAND, 0xFF);
    out = port_read(DRIVERS_KEYBOARD_PORT_STATUS);
    if (out != 0xFA) {
        PANIC(out, "PS2 reset failed.");
    }
}

#endif