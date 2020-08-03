%include "stub.asm"

[BITS 32]

extern kernel_core_entry
extern reload_idt_table
global call_main

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


    call_main:
        push ebp
        mov ebp, esp

        mov eax, [ebp + 0x8]         ; (argc)
        mov ebx, [ebp + 0xc]         ; (argv)

        mov eax, esp
        mov [kernel_saved_stack_top], eax

        ; Preparing for exec.

        mov ax, 0x30
        mov es, ax
        mov ss, ax
        mov ds, ax
        mov fs, ax
        mov gs, ax

        mov eax, 0xFFFF
        mov esp, eax
        call 0x28:0x0000
        ; eax should contain the program return value.

        ; Returned from exec.

        mov bx, 0x10
        mov es, bx
        mov ss, bx
        mov ds, bx
        mov fs, bx
        mov gs, bx

        mov ebx, [kernel_saved_stack_top]
        mov esp, ebx

        mov esp, ebp
        pop ebp
        ret

[SECTION .data]
    kernel_saved_stack_top  db  '    '