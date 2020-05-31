; Fuzzy Bootloader Stage 2
%include "constants.asm"
%include "io.asm"

[BITS 16]

extern entry_stage

global _low_put_chars
global _low_move_xy
global _low_clear_screen
global _low_sleep

[SECTION .text]
        CLI
        MOV ax, 0x0000
        MOV es, ax                  ; es := 0
        set_blinking 0

        print_string_ext bl_stage_2, bl_stage_2_len, 04, 09, C_WHITE, C_BLACK, 0
        call entry_stage
        JMP label_exit

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

    _low_clear_screen:
        push ebp
        mov ebp, esp

        mov ah, 0x06                  ; (scroll)
        mov al, 0x00                  ; (line count)
        mov bh, [ebp + 0x08]          ; (attribute)
        mov cl, [ebp + 0x0c]          ; (window top-left x)
        mov ch, [ebp + 0x10]          ; (window top-left y)
        mov dl, [ebp + 0x14]          ; (window bottom-right x)
        mov dh, [ebp + 0x18]          ; (window bottom-right y)
        int 0x10

        mov esp, ebp
        pop ebp
        ret

    _low_sleep:
        push ebp
        mov ebp, esp
        mov ebx, [ebp + 0x08]          ; (cycles)
        _low_sleep_internal:
        sub ebx, 1
        jnc _low_sleep_internal

        mov esp, ebp
        pop ebp
        ret

label_exit:
        HLT
        JMP label_exit

[SECTION .data]
    bl_stage_2             db      "Bootloader: Stage 2"
    bl_stage_2_len         equ       ($-bl_stage_2)