; Fuzzy Bootloader Stage 2
%include "constants.asm"
%include "io.asm"
%include "io_syscall.asm"
%include "time_syscall.asm"

[BITS 16]

extern entry_stage

[SECTION .text]
        CLI
        MOV ax, 0x0000
        MOV es, ax                  ; es := 0
        set_blinking 0

        print_string_ext bl_stage_2, bl_stage_2_len, 04, 09, C_WHITE, C_BLACK, 0
        call entry_stage
        JMP label_exit

label_exit:
        HLT
        JMP label_exit

        PLUGIN_SYSCALLS_IO
        PLUGIN_SYSCALLS_TIME

[SECTION .data]
    bl_stage_2             db      "Bootloader: Stage 2"
    bl_stage_2_len         equ       ($-bl_stage_2)