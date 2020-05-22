; Fuzzy Bootloader Stage 2
%include "constants.asm"
%include "io.asm"

[ORG 0x8000]
[BITS 16]


[SECTION .text]

        MOV ax, 0x0000
        MOV es, ax                  ; es := 0

        set_blinking 0
        print_string_ext bl_stage_2, bl_stage_2_len, 05, 04, C_WHITE, C_BLACK, 0

        JMP label_exit

    _low_print:
        push bp
        mov bp, sp
        print_string_ext bl_stage_2, bl_stage_2_len, 02, 02, C_BLACK, C_WHITE, 0
        mov ax, 0
        leave
        ret

label_exit:
        HLT

[SECTION .data]
    bl_stage_2             db      "We are at Bootloader: Stage 2"
    bl_stage_2_len         equ       ($-bl_stage_2)