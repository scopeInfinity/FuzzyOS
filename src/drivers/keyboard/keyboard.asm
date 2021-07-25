[BITS 32]

global port_write
global port_read

[SECTION .text]
    port_write:
        push ebp
        mov ebp, esp

        mov al,  [ebp + 0xc]   ; value
        mov dx,  [ebp + 0x8]   ; port
        out dx, al

        pop ebp
        ret

    port_read:
        push ebp
        mov ebp, esp

        mov dx,  [ebp + 0x8]   ; port
        in al, dx
        and eax, 0xFF

        pop ebp
        ret
