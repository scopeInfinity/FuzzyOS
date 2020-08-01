; Fuzzy Bootloader Stage 2
%include "constants.asm"
%include "io.asm"
%include "stub.asm"

[BITS 16]

extern entry_stage
global _low_get_gdtr_address
global enter_protected_mode
global label_exit

[SECTION .text]
        MOV ax, 0x0000
        MOV es, ax                  ; es := 0
        set_blinking 0
        print_string_ext bl_stage_2, bl_stage_2_len, 04, 09, C_WHITE, C_BLACK, 0
        call entry_stage

    _low_get_gdtr_address:
        push ebp
        mov ebp, esp

        get_gdtr_address

        mov esp, ebp
        pop ebp
        ret


    enter_protected_mode:
        ; Never returns.

        ; Load GDT Table
        get_gdtr_address
        lgdt [eax]

        ; Enter Protected mode
        set_protected_mode_entry_address_frm 0
        mov eax, cr0
        or eax, 0x00000001
        mov cr0, eax
        jmp 0x08:0x0000     ; address of smart kernel init

    label_exit:
        HLT
        JMP label_exit


[SECTION .data]
    bl_stage_2             db      "Bootloader: Stage 2"
    bl_stage_2_len         equ       ($-bl_stage_2)