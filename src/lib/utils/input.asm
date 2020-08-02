[BITS 32]

global getch_low

[SECTION .text]
    getch_low:
        push ebp
        mov ebp, esp


        ; We are expecting interrupt to set return value in eax
        mov eax, 0
        int 0x60
        mov esp, ebp
        pop ebp
        ret