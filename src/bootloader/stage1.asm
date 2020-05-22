; Fuzzy Bootloader Stage 1

; Assumptions:
;  - Stage 2 starts from Sector 1 to Sector 1
;  - Stage 2 has ORG 0x8000

%include "constants.asm"
%include "io.asm"
%include "disk.asm"

[ORG 0x7C00]
[BITS 16]


[SECTION .text]

        CLI
        MOV ax, 0x0000
        MOV es, ax                  ; es := 0

        set_blinking 0
        print_string_ext bl_welcome, bl_welcome_len, 02, 02, C_GREEN, C_WHITE, 0
        print_string_ext bls1, bls1_len, 05, 04, C_WHITE, C_BLACK, 0

        ; Attempt to load Bootloader Stage 2 in Memory
        disk_read 01, 80, 00, 00, 02, 8000
        disk_success 80
        JNC label_bts2_loaded
        print_string_ext bls2_load_fail, bls2_load_fail_len, 05, 06, C_RED, C_BLACK, 0
        JMP label_exit

label_bts2_loaded:
        print_string_ext bls2_loaded, bls2_loaded_len, 05, 06, C_WHITE, C_BLACK, 0
        JMP 0x8000
        JMP label_exit

label_exit:
        HLT

[SECTION .data]
    bl_welcome             db      " Fuzzy OS... (^_^) "
    bl_welcome_len         equ       ($-bl_welcome)
    bls1                   db      "Bootloader: Stage 1"
    bls1_len               equ       ($-bls1)
    bls2_loaded            db      "Stage 2 Loaded."
    bls2_loaded_len        equ       ($-bls2_loaded)
    bls2_load_fail         db      "Stage 2 Load Failed!"
    bls2_load_fail_len     equ       ($-bls2_load_fail)
