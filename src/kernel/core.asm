%include "stub.asm"

[BITS 32]

extern kernel_core_entry
extern reload_idt_table
global kernel_core_entry_asm

[SECTION .text]
        ; protected mode real entry point.
        CLI
        mov ax, 0x10
        mov es, ax
        mov ss, ax
        mov ds, ax
        mov fs, ax
        mov gs, ax

        ; transfer control to the desired entry point.
        get_protected_mode_entry
        cmp eax, 0
        je kernel_core_entry

        unshelve_protected_mode_and_ret_entry_address
        push eax
        call reload_idt_table
        STI
        pop eax
        jmp eax

        ; kernel_core_entry_asm currently exists only for tests.
    kernel_core_entry_asm:
        ; __TEST_INJECT_KERNEL_CORE_ENTRY__: mov eax, 0x922E52FF
        ; __TEST_INJECT_KERNEL_CORE_ENTRY__: HLT
        ret