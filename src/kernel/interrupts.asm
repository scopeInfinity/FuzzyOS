[BITS 32]

global interrupt_nohup
global load_idt_table_low
global kernel_enable_interrupts
global kernel_disable_interrupts
global idt_table

global syscall_selector_low
extern syscall_selector
global syscall_copy_es_to_ss

[SECTION .text]

    interrupt_nohup:
        iret

    load_idt_table_low:
        push ebp
        mov ebp, esp

        mov eax, [ebp + 0x8]
        lidt [eax]

        mov esp, ebp
        pop ebp
        ret

    kernel_enable_interrupts:
        STI
        ret

    kernel_disable_interrupts:
        CLI
        ret

    syscall_selector_low:
        ; Not saving SS
        push ds
        push es
        push fs
        push gs
        push ebx
        mov bx, 0x10
        mov ds, bx
        ; mov es, bx  ; ES will point to user space data.
        mov fs, bx
        mov gs, bx
        pop ebx
        push esi
        push edx
        push ecx
        push ebx
        push eax
        call syscall_selector
        add esp, 20
        pop gs
        pop fs
        pop es
        pop ds
        iret

    syscall_copy_es_to_ss:
        ; TODO
        ret
