global _low_put_char
global _low_vga_copy_step

%macro  PLUGIN_SYSCALLS_IO_PROTECTED 0

_low_put_char:
    push ebp
    mov ebp, esp

    mov ebx,[ebp + 0x10]        ; (ROW_WIDTH*y+x)
    shl ebx, 1
    add ebx,0xb8000
    mov al, [ebp + 0x8]         ; (char)
    mov ah, [ebp + 0xc]         ; (color)
    mov [ebx], ax

    mov esp, ebp
    pop ebp
    ret

_low_vga_copy_step:
    push ebp
    mov ebp, esp
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

    mov esp, ebp
    pop ebp
    ret
    
%endmacro