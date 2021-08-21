[BITS 16]

global _low_int_0x13

[SECTION .text]
    _low_int_0x13:
        push ebp
        mov ebp, esp
        ; callee save register
        push ebx
        push esi
        push edi
        push es

        ; check es
        mov ax, [ebp + 0x18]        ; (es)
        mov es, ax

        mov ax, [ebp + 0x08]
        mov bx, [ebp + 0x0C]
        mov cx, [ebp + 0x10]
        mov dx, [ebp + 0x14]
        int 0x13

        pop es
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
