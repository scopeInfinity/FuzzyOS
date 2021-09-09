[BITS 32]

global syscall_strncpy_user_to_kernel
global syscall_strncpy_kernel_to_user
global kernel_memncpy_absolute

[SECTION .text]

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

    kernel_memncpy_absolute:
        ; assumes count is multiple of 4
        push ebp
        mov ebp, esp
        ; callee save register
        push ebx
        push esi
        push edi
        push ds
        push es

        mov ecx, [ebp + 0x18]         ; count
        shr ecx, 2                    ; count/4
        mov esi, [ebp + 0x14]         ; src_loc
        mov  ds, [ebp + 0x10]         ; src_ds
        mov edi, [ebp + 0x0C]         ; dest_loc
        mov  es, [ebp + 0x08]         ; dest_ds

        ; strcpy
        rep movsd                     ; move double word

        pop es
        pop ds
        pop edi
        pop esi
        pop ebx

        pop ebp
        ret


[SECTION .data]
    kernel_saved_stack_top  db  '    '
