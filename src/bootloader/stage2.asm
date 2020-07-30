; Fuzzy Bootloader Stage 2
%include "constants.asm"
%include "io.asm"

[BITS 16]

extern entry_stage
global enter_protected_mode
global label_exit

[SECTION .text]
        MOV ax, 0x0000
        MOV es, ax                  ; es := 0
        set_blinking 0

        print_string_ext bl_stage_2, bl_stage_2_len, 04, 09, C_WHITE, C_BLACK, 0
        call entry_stage

    enter_protected_mode:
        ; Args: (gdtr_address)
        ; And never returns.

        push ebp
        mov ebp, esp

        ; Load GDT Table
        mov eax, [esp+8]   ; gdtr_address
        lgdt [eax]         ; Load GDT Table

        ; Enter Protected mode
        mov eax, cr0
        or eax, 0x00000001
        mov cr0, eax
        jmp 0x08:0x0000     ; address to kernel in memory

    label_exit:
        HLT
        JMP label_exit


[SECTION .data]
    bl_stage_2             db      "Bootloader: Stage 2"
    bl_stage_2_len         equ       ($-bl_stage_2)