[BITS 16]

global __low_panic_halt

[SECTION .text]
    __low_panic_halt:
        HLT
        JMP __low_panic_halt