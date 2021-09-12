[BITS 32]

global irq1_handler_low
global irq12_handler_low

[SECTION .text]

    irq1_handler_low:
        ; TODO: send EOI
        iret

    irq12_handler_low:
        ; TODO: send EOI
        iret
