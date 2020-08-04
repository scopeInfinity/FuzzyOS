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

        ; edx is used below.
        mov edx, ebp

        ; Preparing for exec.

        ; Assigning DS and stuff.
        mov es, cx
        mov ss, cx
        mov ds, cx
        mov fs, cx
        mov gs, cx


        ; far jump to main()
        ; stores IP32:CS16 on the very top of the stack
        mov eax, 0xFFFF
        mov esp, eax
        push ebx
        xor ebx, ebx
        push ebx
        ; Temporarily pushing Kernel EBP on user stack.
        push edx
        call far [0xFFF7]
        ; eax should contain the program return value.
        pop edx

        mov bx, 0x10
        mov es, bx
        mov ss, bx
        mov ds, bx
        mov fs, bx
        mov gs, bx

        mov ebp, edx

        mov esp, ebp
        ; kernal stack is valid again
        pop ebp
        ret