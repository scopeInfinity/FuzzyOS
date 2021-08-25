%include "fuzzy/memmgr/layout.asm"

[BITS 32]

global interrupt_nohup
global load_idt_table_low
global kernel_enable_interrupts
global kernel_disable_interrupts
global idt_table

global interrupt_handler_0x32_syscall_handler
extern syscall_selector
global _interrupt_handler_0x00_0x1F_exception
extern interrupt_handler_0x00_0x1F_exception
global syscall_copy_es_to_ss

KERNEL_STACK_MARKER_NEW    EQU 0x10000001
KERNEL_STACK_MARKER_NOTNEW EQU 0x10000000

%macro _int_start 0
        ; NOT READY for HW interrupts
        ;  - eax not preserved.
        ;  - disabled nested, as we aren't sure if it will work.
        CLI  ; should get restored on iret

        ; meant to placed at start of interrupt handler
        push ebp
        mov ebp, esp

        ; general purpose register
        ; push eax
        push ebx
        push ecx
        push edx
        push esi
        push edi
        ; ignore esp; will reset at end of call
        ; ignore ebp; already pushed

        ; below ebp: old+ebp, CS:IP, eflags 16 bytes
        mov ebx, [ebp + 0x10]
        mov ecx, [ebp + 0x14]
        mov edx, [ebp + 0x18]
        mov esi, [ebp + 0x1C]
        mov edi, [ebp + 0x20]

        ; segment registers

        ; HANDLE_SS_START
        ; if ss is not already in kernel mode than create kernel stack
        ; and save ongoing stack.
        mov ax, 0x10
        push ebx
        mov bx, ss
        cmp bx, ax
        pop ebx
        je _kernel_stack_new_not_needed
        ; create new kernel stack
        mov ebp, esp
        mov ss, ax
        mov esp, STACKINIT_KERNEL_EVENT
        push ebp    ; old esp ; ASSUME ss == ds
        mov ebp, esp
        mov eax, KERNEL_STACK_MARKER_NEW
        push eax
        jmp _kernel_stack_done
    _kernel_stack_new_not_needed:
        mov eax, KERNEL_STACK_MARKER_NOTNEW
        push eax
    _kernel_stack_done:
        ; HANDLE_SS_END

        ; cs: already pushed when interrupt triggered
        ; ss: there is no benefit to push ss on last stack
        push ds
        push es
        push fs
        push gs

        ; Let's move to kernel mode and move interrupt arguments as function argument
        ; es: will updated in next section, and will be es==ds
        mov ax, 0x10
        mov ds, ax
        mov fs, ax
        mov gs, ax

        mov eax, es
        push eax  ; arg5: user_dataspace/${last}_dataspace es==ds
        mov eax, ds
        mov es, eax

        push edi  ; arg4
        push esi  ; arg3
        push edx  ; arg2
        push ecx  ; arg1
        push ebx  ; arg0
%endmacro

%macro _int_end 0
        ; meant to placed at end of interrupt handler
        ; and must NOT update eax
        add esp, 24

        ; Let's remove to previous mode
        ; restore segment register, cs and ss are left
        pop gs
        pop fs
        pop es
        pop ds

        ; HANDLE_SS_START

        ; move stack to previous stack if it wasn't kernel stack
        pop ebx
        cmp ebx, KERNEL_STACK_MARKER_NOTNEW
        je _kernel_stack_skip_no_new
        ; move to old stack
        pop esp
        mov bx, ds
        ; assumption ss == ds
        ; it might break in nested hardware interrupts.
        mov ss, bx
    _kernel_stack_skip_no_new:
        ; HANDLE_SS_END

        ; restore general purpose flag
        pop edi
        pop esi
        pop edx
        pop ecx
        pop ebx
        ; TODO: eax value must be preserve for full routine for HW interrupts.
        ; eax value must remain same during _int_end for syscall.
        ; pop eax
        pop ebp
%endmacro

[SECTION .text]

    interrupt_nohup:
        iret

    load_idt_table_low:
        push ebp
        mov ebp, esp

        mov eax, [ebp + 0x8]
        lidt [eax]

        pop ebp
        ret

    kernel_enable_interrupts:
        STI
        ret

    kernel_disable_interrupts:
        CLI
        ret

    interrupt_handler_0x32_syscall_handler:
        _int_start
        call syscall_selector
        _int_end
        iret

    _interrupt_handler_0x00_0x1F_exception:
        call interrupt_handler_0x00_0x1F_exception
        HLT
