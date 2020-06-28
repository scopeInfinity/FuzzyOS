global _low_put_chars
global _low_move_xy
global _low_scroll_screen
global _low_read_char

%macro  PLUGIN_SYSCALLS_IO 0

_low_put_chars:
    push ebp
    mov ebp, esp

    mov ah, 0x09                 ; (write)
    mov al, [ebp + 0x8]          ; (char)
    mov cx, [ebp + 0xc]          ; (count)
    mov bh, 0x00                 ; (pagenumber)
    mov bl, [ebp + 0x10]         ; (attribute)
    int 0x10

    mov esp, ebp
    pop ebp
    ret

_low_move_xy:
    push ebp
    mov ebp, esp

    mov ah, 0x02                 ; (set custor position)
    mov dl, [ebp + 0x8]          ; (pos_x)
    mov dh, [ebp + 0xc]          ; (pos_y)
    mov bh, [ebp + 0x10]         ; (page number)
    int 0x10

    mov esp, ebp
    pop ebp
    ret

_low_scroll_screen:
    push ebp
    mov ebp, esp

    mov ah, 0x06                  ; (scroll)
    mov al, [ebp + 0x08]          ; (line count)
    mov bh, [ebp + 0x0c]          ; (attribute)
    mov cl, [ebp + 0x10]          ; (window top-left x)
    mov ch, [ebp + 0x14]          ; (window top-left y)
    mov dl, [ebp + 0x18]          ; (window bottom-right x)
    mov dh, [ebp + 0x1c]          ; (window bottom-right y)
    int 0x10

    mov esp, ebp
    pop ebp
    ret

_low_read_char:
    push ebp
    mov ebp, esp

    mov ah, 0x00                  ; (read character)
    int 0x16
    and eax, 0xFF

    mov esp, ebp
    pop ebp
    ret


%endmacro