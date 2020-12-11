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
        clear_screen C_WHITE, C_BLACK
        print_string_ext bl_welcome, bl_welcome_len, 02, 02, C_MAGENTA, C_WHITE, 0
        print_string_ext bls1, bls1_len, 04, 04, C_WHITE, C_BLACK, 0

        ; Attempt to load Bootloader Stage 2 in Memory
        disk_read SECTOR_COUNT_BT_STAGE2, 80, 00, 00, SECTOR_START_BT_STAGE2, 8000
        disk_success 80
        JNC label_bts2_loaded
        print_string_ext bls2_load_fail, bls2_load_fail_len, 06, 06, C_RED, C_BLACK, 0
        JMP label_exit

label_bts2_loaded:
        print_string_ext bls2_loaded, bls2_loaded_len, 06, 07, C_WHITE, C_BLACK, 0
        print_hex_string_ext 0x8000, 10, C_WHITE, 0
        ; __TEST_INJECT_BT1__: mov eax, 0x9A11C824
        ; __TEST_INJECT_BT1__: HLT
        JMP 0x8000
        JMP label_exit

label_exit:
        HLT
        JMP label_exit

[SECTION .data]
    bl_welcome             db      " Fuzzy OS... (^_^) "
    bl_welcome_len         equ       ($-bl_welcome)
    bls1                   db      "Bootloader: Stage 1"
    bls1_len               equ       ($-bls1)
    bls2_loaded            db      "Stage 2 Loaded: "
    bls2_loaded_len        equ       ($-bls2_loaded)
    bls2_load_fail         db      "Stage 2 Load Failed!"
    bls2_load_fail_len     equ       ($-bls2_load_fail)
