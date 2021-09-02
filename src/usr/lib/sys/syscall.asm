[BITS 32]

global syscall

[SECTION .text]

    syscall:
        push ebp
        mov ebp, esp
        ; callee save register
        push ebx
        push esi
        push edi

        mov ebx, [ebp + 0x08]         ; (id)
        mov ecx, [ebp + 0x0c]         ; (arg0)
        mov edx, [ebp + 0x10]         ; (arg1)
        mov esi, [ebp + 0x14]         ; (arg2)
        mov edi, [ebp + 0x18]         ; (arg3)

        pushf
        push edi
        push esi
        push edx
        push ecx
        push ebx
        ; CLI hack is introduced to avoid race condition when
        ;   just after int 0x32 and before CLI (inside interrupt handle)
        ;   if the IRQ0 get triggered, it will fetch kernel code segment
        ;   and use it to determine invalid PID.
        ; Also, user application should not have previlage to execute CLI.
        ; It should not be that complicated to fix, I hope.
        CLI
        int 0x32
        add esp, 20

        popf
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
