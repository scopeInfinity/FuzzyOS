[BITS 32]

global panic_just_halt

[SECTION .text]
    panic_just_halt:
        HLT
        JMP panic_just_halt