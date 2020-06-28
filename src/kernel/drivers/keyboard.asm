%macro  PLUGIN_KERNEL_DRIVERS_KEYBOARD 0

global port_write
global port_read

[SECTION .text]
    port_write:
        push ebp
        mov ebp, esp
        
        mov al,  [ebp + 0xc]   ; value
        mov dx,  [ebp + 0x8]   ; port
        out dx, al

        mov esp, ebp
        pop ebp
        ret

    port_read:
        push ebp
        mov ebp, esp
        
        mov dx,  [ebp + 0x8]   ; port
        in al, dx
        and eax, 0xFF

        mov esp, ebp
        pop ebp
        ret

%endmacro