#include <fuzzy/drivers/ps2/ps2.h>
#include <fuzzy/kernel/panic.h>
#include <fuzzy/kernel/interrupts/interrupts.h>
#include <fuzzy/drivers/port.h>

#include <lib/utils/logging.h>

#include <stddef.h>

#define LOG_PREFIX "[drivers][ps2] "


#define STATUS_OUTPUT_BUFFER_INDEX 0
#define STATUS_INPUT_BUFFER_INDEX  1
#define STATUS_SYSTEM_FLAG_INDEX   2

#define FULL 1
#define EMPTY 0

#define CMD_PORT1_DISABLE            0xAD
#define CMD_PORT1_ENABLE             0xAE
#define CMD_PORT2_DISABLE            0xA7
#define CMD_PORT2_ENABLE             0xA8
#define CMD_TEST_PS2_CONTROLLER      0xAA
#define CMD_TEST_PORT1               0xAB
#define CMD_TEST_PORT2               0xA9
#define CMD_CONTROLLER_CONFIG_READ   0x20
#define CMD_CONTROLLER_CONFIG_WRITE  0x60

#define DATA_DEVICE_RESET            0xFF
#define DATA_DEVICE_DISABLE_SCAN     0xF5
#define DATA_DEVICE_IDENTIFY         0xF2

// Potential improvements: retry or timeout

void ps2_controller_wait_for_empty_input() {
    while(((inputb(PORT_PS2_STATUS)>>STATUS_INPUT_BUFFER_INDEX) & 1 ) == FULL) {
        // waiting
    }
    // input buffer should be empty now.
}

void ps2_controller_wait_for_full_output() {
    while(((inputb(PORT_PS2_STATUS)>>STATUS_OUTPUT_BUFFER_INDEX) & 1 ) == EMPTY) {
        // waiting
    }
    // output buffer should not be empty now.
}

static void ps2_cmd(uint8_t cmd) {
    ps2_controller_wait_for_empty_input();
    outb(PORT_PS2_CMD, cmd);
}

static void ps2_cmd2(uint8_t cmd, uint8_t byte) {
    ps2_controller_wait_for_empty_input();
    outb(PORT_PS2_CMD, cmd);
    ps2_controller_wait_for_empty_input();
    outb(PORT_PS2_DATA, byte);
}

void ps2_write_port1(uint8_t byte) {
    // if issues occur, add timeout or retry.
    ps2_controller_wait_for_empty_input();
    outb(PORT_PS2_DATA, byte);
}

uint8_t ps2_read_data() {
    ps2_controller_wait_for_full_output();
    return inputb(PORT_PS2_DATA);
}

void ps2_init() {
    uint8_t out;

    print_log(LOG_PREFIX "initializing...");

    print_log(LOG_PREFIX "disable ps/2 port");
    ps2_cmd(CMD_PORT1_DISABLE);
    ps2_cmd(CMD_PORT2_DISABLE);

    print_log(LOG_PREFIX "flush output buffer");
    inputb(PORT_PS2_DATA);

    print_log(LOG_PREFIX "write controller configuration");
    ps2_cmd(CMD_CONTROLLER_CONFIG_READ);
    uint8_t controller_config = ps2_read_data();
    // disable first port interrupt
    // disable second port interrupt
    // disable first port translation
    controller_config = controller_config&(~0b01000011);
    ps2_cmd2(CMD_CONTROLLER_CONFIG_WRITE, controller_config);

    print_log(LOG_PREFIX "ps/2 controller self test");
    ps2_cmd(CMD_TEST_PS2_CONTROLLER);
    out = ps2_read_data();
    if (out != 0x55) {
        PANIC(out, "PS/2 controller self test failed.");
    }

    print_log(LOG_PREFIX "rewrite controller configuration");
    ps2_cmd2(CMD_CONTROLLER_CONFIG_WRITE, controller_config);

    print_log(LOG_PREFIX "checking dual ps/2 port");
    int is_port2_supported = 0;
    {
        // we have already marked port 2 as disabled above
        int is_port2_disabled = (controller_config&(1<<5))>0?1:0;
        if(!is_port2_disabled) {
            // port should be disabled but it doesn't look like that
            // so assuming as single channel.
        } else {
            // checking more
            ps2_cmd(CMD_PORT2_ENABLE);
            ps2_cmd(CMD_CONTROLLER_CONFIG_READ);
            uint8_t _controller_config = ps2_read_data();
            is_port2_disabled = (_controller_config&(1<<5))>0?1:0;
            if(is_port2_disabled) {
                // port should be enabled but it doesn't look like that
                // so assuming as single channel.
            } else {
                is_port2_supported = 1;
                ps2_cmd(CMD_PORT2_DISABLE);
            }
        }
    }
    print_log(LOG_PREFIX "does ps/2 has dual channel? %s", is_port2_supported?"Yes":"No");

    print_log(LOG_PREFIX "test ps/2 port 1");
    ps2_cmd(CMD_TEST_PORT1);
    out = ps2_read_data();
    if (out != 0x00) {
        // port 1 is expected to be connected for now.
        PANIC(out, "PS/2 controller port 1 test failed.");
    }

    if(is_port2_supported) {
        print_log(LOG_PREFIX "test ps/2 port 2");
        ps2_cmd(CMD_TEST_PORT2);
        out = ps2_read_data();
        if (out != 0x00) {
            // port 2 is expected to be connected for now.
            PANIC(out, "PS/2 controller port 2 test failed.");
        }
    }

    print_log(LOG_PREFIX "enable ports");
    ps2_cmd(CMD_PORT1_ENABLE);
    if(is_port2_supported) {
        ps2_cmd(CMD_PORT2_ENABLE);
    }

    // Interrupt method is not yet used.
    // print_log(LOG_PREFIX "enable port interrupts");
    // // enable first port interrupt
    // controller_config = controller_config|(0b00000001);
    // if(is_port2_supported) {
    //     // enable second port interrupt
    //     controller_config = controller_config|(0b00000010);
    // }
    // ps2_cmd2(CMD_CONTROLLER_CONFIG_WRITE, controller_config);

    print_log(LOG_PREFIX "device reset");
    ps2_write_port1(DATA_DEVICE_RESET);
    out = ps2_read_data();
    if (out!=0xFA) {
        PANIC(out, LOG_PREFIX "device reset failed");
    }
    out = ps2_read_data();
    if (out!=0xAA) {
        PANIC(out, LOG_PREFIX "device reset; port 1 self test failed");
    }

    print_log(LOG_PREFIX "[port1] disable device scanning");
    ps2_write_port1(DATA_DEVICE_DISABLE_SCAN);
    out = ps2_read_data();
    if (out!=0xFA) {
        PANIC(out, LOG_PREFIX "[port1] disable device scanning failed");
    }

    print_log(LOG_PREFIX "[port1] identify device");
    ps2_write_port1(DATA_DEVICE_IDENTIFY);
    out = ps2_read_data();
    if (out!=0xFA) {
        PANIC(out, LOG_PREFIX "[port1] identify device failed");
    }
    out = ps2_read_data();  // device type first byte
    // ignoring second byte data, it should get cleared soon.
    if(out != 0xAB) {
        PANIC(out, "[port1] expected device keyboard not found.");
    }
    print_log(LOG_PREFIX "[port1] keyboard found");
    return;
}

extern irq1_handler_low();
extern irq12_handler_low();

void interrupt_register_0x21_0x2C_irq1_ir12_keyboard_mouse() {
    populate_idt_entry_32bit(IDT_IRQ1_KEYBOARD, (unsigned int)irq1_handler_low, 0, 0);
    populate_idt_entry_32bit(IDT_IRQ12_MOUSE, (unsigned int)irq12_handler_low, 0, 0);
}

void irq1_handler() {
    // port 1
}

void irq12_handler() {
    // port 2
}
