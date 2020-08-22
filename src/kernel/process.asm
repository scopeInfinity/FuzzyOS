[BITS 32]

global call_main

[SECTION .text]

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