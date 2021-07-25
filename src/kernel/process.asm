[BITS 32]

global call_main
global syscall_strncpy_user_to_kernel
global syscall_strncpy_kernel_to_user

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

        ; assigning segment registers
        mov es, cx
        mov ss, cx
        mov ds, cx
        mov fs, cx
        mov gs, cx

        ; assign kernel stack
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

    syscall_strncpy_user_to_kernel:
        push ebp
        mov ebp, esp
        ; callee save register
        push ebx
        push esi
        push edi
        push ds

        mov eax, [ebp + 0x08]         ; user_ds
        mov ds, eax

        ; strcpy
        mov esi, [ebp + 0x0C]         ; ds:esi, char *src_es_address
        mov edi, [ebp + 0x10]         ; es:edi, char *dest_ds_address
        mov ecx, [ebp + 0x14]         ; size_t size
        rep movsb

        pop ds
        pop edi
        pop esi
        pop ebx

        pop ebp
        ret

    syscall_strncpy_kernel_to_user:
        push ebp
        mov ebp, esp
        ; callee save register
        push ebx
        push esi
        push edi
        push es

        mov eax, [ebp + 0x08]         ; user_ds
        mov es, eax

        ; strcpy
        mov edi, [ebp + 0x0C]         ; es:edi, char *dest_address
        mov esi, [ebp + 0x10]         ; ds:esi, char *src_address
        mov ecx, [ebp + 0x14]         ; size_t size
        rep movsb

        pop es
        pop edi
        pop esi
        pop ebx

        pop ebp
        ret


[SECTION .data]
    kernel_saved_stack_top  db  '    '
