#pragma once

#include <stddef.h>

#define PORT_PIC1_CMD     0x20
#define PORT_PIC1_DATA    0x21
#define PORT_PIC2_CMD     0xA0
#define PORT_PIC2_DATA    0xA1

#define PORT_PIT_DATA0    0x40
#define PORT_PIT_DATA1    0x41
#define PORT_PIT_DATA2    0x42
#define PORT_PIT_CMD      0x43

static inline void outb(uint16_t port, uint8_t data) {
    __asm__ volatile(
        "outb %0, %1 \n"
        :  /* output */
        : "a" (data), "ir" (port)  /* input */
        :
    );
}

static inline uint8_t inputb(uint16_t port) {
    uint8_t data;
    __asm__ volatile(
        "inb %1, %0 \n"
        : "=a" (data) /* output */
        : "ir" (port)  /* input */
        :
    );
    return data;
}
