[BITS 32]

global call_main

[SECTION .text]

    call_main:
        ; TODO: write assumptions and constraints
        ; TODO: function needs to be verified for exit case.
        push ebp
        mov ebp, esp

        mov eax, [ebp + 0x10]         ; (argc)
        mov eax, [ebp + 0x14]         ; (argv)

        ; bx and cx are used below.
        mov ebx, [ebp + 0x08]         ; (CS)
        mov ecx, [ebp + 0x0c]         ; (DS)

        mov eax, esp
        mov [kernel_saved_stack_top], eax


        ; Preparing for exec.

        ; Assigning DS and stuff.
        mov es, cx
        mov ss, cx
        mov ds, cx
        mov fs, cx
        mov gs, cx

        ; fix app kernel stack
        mov eax, 0xFFFC
        mov esp, eax


        ; far jump to main()
        push ebx    ; CS, 2 bytes
        xor ebx, ebx
        push ebx    ; IP: 4 bytes
        call far [esp]

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
    farjmp_location dd 0
                    dw 0