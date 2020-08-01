%include "stub.asm"

; Code must be within 256 bytes.
[ORG 0x7E00]
[BITS 16]

[SECTION .text]
        jmp execute_0x13_enter_real_mode

    execute_0x13_enter_real_mode:
        CLI
        mov eax, cr0
        and eax, 0xFFFFFFFE
        mov cr0, eax
        jmp 0x0:execute_0x13_from_real_mode

    execute_0x13_from_real_mode:
        ; minimal setup for real mode as required.
        mov ax, 0
        mov ds, ax
        lidt [idt_table_real_mode]
        ; execute the interrupt
        unshelve_values_from_real_mode 0, ebx
        mov es, bx
        unshelve_values_from_real_mode 1, eax
        unshelve_values_from_real_mode 2, ebx
        unshelve_values_from_real_mode 3, ecx
        unshelve_values_from_real_mode 4, edx
        int 0x13
        shelve_values_from_real_mode 1, eax
        ; set_protected_mode_entry_address should be already be called even before entering the real mode.
        mov eax, cr0
        or eax, 0x00000001
        mov cr0, eax
        mov [0x7EF0], eax

        jmp 0x08:0x0000     ; real address of smart kernel init

[SECTION .data]
idt_table_real_mode     dw  1024
                        db  0