#include <fuzzy/drivers/pic/pit.h>
#include <fuzzy/drivers/port.h>

static uint16_t frequency_dividor;

void pit_init() {
    // Channel 0
    // Low/high byte
    // Interrupt on terminal count
    outb(PORT_PIT_CMD, 0b00110000);
}

uint16_t pit_get_counter() { return frequency_dividor; }

void pit_reload_counter() {
    uint16_t counter = pit_get_counter();
    outb(PORT_PIT_DATA0, counter & 0xFF);
    outb(PORT_PIT_DATA0, counter >> 8);
}

void pit_reset() {
    outb(PORT_PIC1_CMD, 0x20);
    outb(PORT_PIC2_CMD, 0x20);
    pit_reload_counter();
}

void pit_set_counter(uint16_t counter) {
    frequency_dividor = counter;
    pit_reload_counter();
}
