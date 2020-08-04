[BITS 32]

global syscall

[SECTION .text]

    syscall:
        push ebp
        mov ebp, esp

        mov eax, [ebp + 0x08]         ; (id)
        mov ebx, [ebp + 0x0c]         ; (arg0)
        mov ecx, [ebp + 0x10]         ; (arg1)
        mov edx, [ebp + 0x14]         ; (arg2)
        mov esi, [ebp + 0x18]         ; (arg3)
        int 0x32

        mov esp, ebp
        pop ebp
        ret
