%include "fuzzy/memmgr/tables/gdt.asm"

[BITS 32]

global _low_put_char
global _low_vga_copy_step
global _low_flush

[SECTION .text]
    _low_put_char:
        push ebp
        mov ebp, esp
        ; callee save register
        push ebx
        ; push esi
        ; push edi

        push ds
        mov eax, GDT_ABS32_DS
        mov ds, eax                 ; Absolute memory address

        mov ebx,[ebp + 0x10]        ; (ROW_WIDTH*y+x)
        shl ebx, 1
        add ebx, 0xb8000
        mov al, [ebp + 0x8]         ; (char)
        mov ah, [ebp + 0xc]         ; (color)
        mov [ebx], ax

        pop ds
        ; pop edi
        ; pop esi
        pop ebx
        pop ebp
        ret

    _low_vga_copy_step:
        push ebp
        mov ebp, esp
        ; callee save register
        push ebx
        ; push esi
        ; push edi
        push ds
        mov eax, GDT_ABS32_DS
        mov ds, eax                 ; Absolute memory address

        ; Copy char+colors in Row Order Format
        mov eax,[ebp + 0x8]    ; (ROW_WIDTH*y1+x1)
        mov ebx,[ebp + 0xc]    ; (ROW_WIDTH*y2+x2)
        mov ecx,[ebp + 0x10]   ; (bytes_count/2)
        add eax,0xb8000
        add ebx,0xb8000

        _low_vga_copy_step_internal:
        mov dx, [eax]
        mov [ebx], dx
        add eax, 2
        add ebx, 2
        loop _low_vga_copy_step_internal

        pop ds
        ; pop edi
        ; pop esi
        pop ebx
        pop ebp
        ret

    _low_flush:
        push ebp
        mov ebp, esp
        ; callee save register
        ; push ebx
        push esi
        push edi
        push es
        mov eax, GDT_ABS32_DS
        mov es, eax                 ; Absolute memory address

        mov esi, [ebp + 0x8]    ; (buffer)
        mov ecx, [ebp + 0xc]    ; (count)
        mov edi, 0xb8000

        cld
        rep movsw

        pop es
        pop edi
        pop esi
        ; pop ebx
        pop ebp
        ret