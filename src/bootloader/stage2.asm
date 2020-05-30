; Fuzzy Bootloader Stage 2
%include "constants.asm"
%include "io.asm"

    [BITS 16]

global _low_print
extern entry_stage2

[SECTION .text]
        CLI
        MOV ax, 0x0000
        MOV es, ax                  ; es := 0
        set_blinking 0

        print_string_ext bl_stage_2, bl_stage_2_len, 04, 09, C_WHITE, C_BLACK, 0
        call entry_stage2

        JMP label_exit

    _low_print:
        push ebp
        mov ebp, esp

        ; check es, bx
        mov ax, 0x1301              ; (print string, update cursor)
        mov bh, 0                   ; (pagenumber)
        mov bl, [ebp + 0x18]        ; (attribute)
        mov dh, [ebp + 0x14]        ; (pos_y)
        mov dl, [ebp + 0x10]        ; (pos_x)
        mov ecx, [ebp + 0xc]        ; (string length)
        mov ebp, [ebp + 0x8]        ; (es:bp string pointer)
        int 0x10

        mov ax, 0
        mov esp, ebp
        pop ebp
        ret

label_exit:
        HLT
        JMP label_exit

[SECTION .data]
    bl_stage_2             db      "Bootloader: Stage 2"
    bl_stage_2_len         equ       ($-bl_stage_2)