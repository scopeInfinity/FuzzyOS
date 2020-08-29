[BITS 32]

global call_main
global process_shelve
global process_unshelve
global process_prepare_new

global process_manager_esp
global process_esp
global process_ss

extern irq0_interrupt_timer_handler_low_shelved
extern irq0_interrupt_timer_handler_low_unshelved

[SECTION .text]

    _process_scheduler:
        ; Does something great!!!


    _low_delegate_to_process_scheduler:
        ; TODO: do something
        ; eax: exit code
        ; ebx: ds  ; for process id reverse lookup.
        hlt
        mov cx, 0x10
        mov es, cx
        mov ds, cx
        mov ss, cx
        mov fs, cx
        mov gs, cx
        pop ebp
        ret


    _low_process_jmp_running:
        push ebp
        mov ebp, esp

        mov eax, [ebp + 0x08]         ; (CS)
        mov ebx, [ebp + 0x0c]         ; (DS)
        mov ecx, [ebp + 0x10]         ; (IP)
        mov edx, [ebp + 0x14]         ; (SP)
        mov esi, [ebp + 0x18]         ; (BP)

        ; TODO: do something
        ; eax: exit code
        ; ebx: ds  ; for process id reverse lookup.
        xor ebx, ebx
        mov bx, ds
        jmp _low_delegate_to_process_scheduler

    process_shelve:
        ; Using process_unshelve as label instead
        ; of function to avoid use of stack.
        ; int should have pushed cs:ip on stack.
        ; TODO: Fix pushfl complains.
        ;pushfl
        pushad
        xor eax, eax
        mov ax, ds
        push eax  ; push ds

        mov ax, 0x10
        mov es, ax
        mov ds, ax
        mov fs, ax
        mov gs, ax
        ; Save process ESP and
        ; load progress manager SS:ESP.
        mov ebx, esp
        mov [process_esp], ebx
        mov bx, ss
        mov [process_ss], bx

        mov ax, 0x30    ; Process Manager SS
        mov ss, ax
        mov ebx, 0xFFFC ; [process_manager_esp]
        mov esp, ebx
        jmp irq0_interrupt_timer_handler_low_shelved

    process_unshelve:
        ; Using process_unshelve as label instead
        ; of function to avoid use of stack.
        mov eax, [process_esp]
        mov esp, eax
        mov bx, [process_ss]
        mov ss, bx
        mov [0x4600], bx

        ; Expects process SS:ESP to be loaded.
        pop eax  ; pop ds
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        popad
        ;popfl
        ; iret can pop cs:ip appropriately.
        jmp irq0_interrupt_timer_handler_low_unshelved

    process_prepare_new:
        ; returns user program stack
        push ebp
        mov ebp, esp
        mov dx, ss  ; saving ss and esp temporarily.

        ; entering user process stack temporarily.
        mov ecx, [ebp + 0x08]         ; (cs)
        mov ebx, [ebp + 0x0c]         ; (ds)
        mov eax, [ebp + 0x10]         ; (ss)

        mov esp, 0xFFF0  ; keeping some buffer for extras.
        mov ss, ax

        push ecx     ; similar to push cs within int $x
        xor ecx, ecx
        push ecx    ; push IP as 0

        ;pushfl
        pushad
        push ebx  ; equivalent to push ds

        mov eax, esp  ; Top of user program stack

        mov ss, dx   ; restoring ss
        mov esp, ebp ; restoring esp
        pop ebp
        ret

    call_main:
        push ebp
        mov ebp, esp

        mov eax, [ebp + 0x10]         ; (argc)
        mov eax, [ebp + 0x14]         ; (argv)

        ; bx and cx are used below.
        mov ebx, [ebp + 0x08]         ; (CS)
        mov ecx, [ebp + 0x0c]         ; (DS)

        ; edx and si is used below to preserve ebp and ss.
        mov edx, ebp
        mov si, ss

        ; Preparing for exec.

        ; Assigning DS and stuff.
        mov es, cx
        mov ss, cx
        mov ds, cx
        mov fs, cx
        mov gs, cx


        ; far jump to main()
        ; stores IP32:CS16 on the very top of the stack
        mov eax, 0xFFFC
        mov esp, eax
        push ebx
        xor ebx, ebx
        push ebx
        ; Temporarily pushing Kernel EBP and SS on user stack.
        push edx
        push esi
        call far [0xFFF4]
        ; eax should contain the program return value.
        pop esi
        pop edx

        mov bx, 0x10
        mov es, bx
        mov ds, bx
        mov fs, bx
        mov gs, bx

        mov ss, si
        mov ebp, edx

        mov esp, ebp
        ; kernal/in-interrupt stack is valid again
        pop ebp
        ret


[SECTION .data]
    process_manager_esp  dd 0
    process_esp          dd 0
    process_ss           dw 0
