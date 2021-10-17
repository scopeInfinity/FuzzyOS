#pragma once

/*
 * Interrupt descriptive table
 * [00:32) ; [0x00:0x20) ; Exceptions: faults, traps, aborts
 * [32:48) ; [0x20:0x30) ; IRQ0-15
 * 80      ; 0x32        ; syscall
 */
#define IDT_SIZE 256
#define IDT_IRQ_OFFSET 0x20

// Used by /usr/lib/process.c
#define IDT_IRQ0_PIC (IDT_IRQ_OFFSET + 0)
#define IDT_IRQ1_KEYBOARD (IDT_IRQ_OFFSET + 1)
#define IDT_IRQ12_MOUSE (IDT_IRQ_OFFSET + 12)
// Used by /usr/lib/sys/syscall.asm
#define IDT_SYSCALL 0x32

extern void populate_idt_entry_32bit(int id, unsigned int address,
                                     unsigned char dpl, // 2-bit
                                     int is_trap);