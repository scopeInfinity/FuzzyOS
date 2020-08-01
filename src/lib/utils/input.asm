[BITS 32]

global getch

[SECTION .text]
    getch:
        ; We are expecting interrupt to set return value in eax
        int 0x60
        ret