; Fuzzy Bootloader
%include "constants.asm"
%include "io.asm"

[ORG 0x7C00]
[BITS 16]


[SECTION .text]

        cli
        mov ax, 0x0000
        mov es, ax                  ; es := 0          

        set_blinking 0
        print_string_ext bl_welcome, bl_welcome_len, 02, 02, C_BLACK, C_WHITE, 0
        print_string_ext bl_stage_2, bl_stage_2_len, 05, 04, C_WHITE, C_BLACK, 0
        hlt

[SECTION .data]
    bl_welcome             db      "Fuzzy OS... (^_^)"
    bl_welcome_len         equ       ($-bl_welcome)
    bl_stage_2             db      "Loading Bootloader: Stage 2"
    bl_stage_2_len         equ       ($-bl_stage_2)
