; Fuzzy Bootloader Stage 2
%include "constants.asm"
%include "io.asm"

[BITS 16]

extern entry_stage
extern gdtr
global enter_protected_mode
global label_exit

[SECTION .text]
        mov eax, 0x0
        mov es, eax
        mov ss, eax
        mov ds, eax
        mov fs, eax
        mov gs, eax
        mov eax, 0xBFFC  ; create stack
        mov esp, eax

        set_blinking 0
        print_string_ext bl_stage_2, bl_stage_2_len, 04, 09, C_WHITE, C_BLACK, 0
        call entry_stage

    enter_protected_mode:
        ; Never returns.

        ; Load GDT Table
        mov eax, gdtr
        lgdt [eax]

        ; Enter Protected mode
        mov eax, cr0
        or eax, 0x00000001
        mov cr0, eax

        ; __TEST_INJECT_BT2__: mov eax, 0x198A65C3
        ; __TEST_INJECT_BT2__: HLT
        jmp 0x08:0x0000     ; address of smart kernel init

    label_exit:
        HLT
        JMP label_exit


[SECTION .data]
    bl_stage_2             db      "Bootloader: Stage 2"
    bl_stage_2_len         equ       ($-bl_stage_2)