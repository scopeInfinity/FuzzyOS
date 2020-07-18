[BITS 32]

global interrupt_nohup
global load_idt_table
global kernel_enable_interrupts
global kernel_disable_interrupts
global idt_table

[SECTION .text]

    interrupt_nohup:
        iret

    load_idt_table:
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
