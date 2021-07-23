[BITS 32]

global syscall

[SECTION .text]

    syscall:
        push ebp
        mov ebp, esp

        mov ebx, [ebp + 0x08]         ; (id)
        mov ecx, [ebp + 0x0c]         ; (arg0)
        mov edx, [ebp + 0x10]         ; (arg1)
        mov esi, [ebp + 0x14]         ; (arg2)
        mov edi, [ebp + 0x18]         ; (arg3)

        push edi
        push esi
        push edx
        push ecx
        push ebx
        int 0x32 ; EIP: 0x20117
        add esp, 20

        mov esp, ebp
        pop ebp
        ret
