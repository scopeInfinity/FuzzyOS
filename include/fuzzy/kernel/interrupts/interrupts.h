#pragma once

/*
 * Interrupt descriptive table
 * [00:32) ; [0x00:0x20) ; Exceptions: faults, traps, aborts
 * [32:48) ; [0x20:0x30) ; IRQ0-15
 * 80      ; 0x32        ; syscall
 */
#define IDT_SIZE        256
#define IDT_IRQ_OFFSET  0x20
#define IDT_IRQ0_PIC    (IDT_IRQ_OFFSET+0)
#define IDT_SYSCALL     0x32


extern void populate_idt_entry_32bit(int id,
    unsigned int address,
    unsigned char dpl, // 2-bit
    int is_trap
    );