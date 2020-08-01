%include "stub.asm"

[BITS 32]

global _low_disk_read_sectors_real

[SECTION .text]
    _low_disk_read_sectors_real:
        push ebp
        mov ebp, esp

        mov ebx, [ebp + 0x14]        ; (es of write address)
        shelve_values_from_protected_mode 0, ebx
        mov ah, 0x02                ; (read sectors)
        mov al, [ebp + 0x8]         ; (sector count)
        shelve_values_from_protected_mode 1, eax
        mov bx, [ebp + 0x18]        ; (bx as write address)
        shelve_values_from_protected_mode 2, ebx
        mov cx, [ebp + 0xc]         ; (cylinder 10bit, sector 6bit)
        shelve_values_from_protected_mode 3, ecx
        mov dx, [ebp + 0x10]        ; (head, drive index)
        shelve_values_from_protected_mode 4, edx

        shelve_protected_mode_with_entry_address_fpm _low_disk_read_sectors_i1
        execute_real_mode_0x13

    _low_disk_read_sectors_i1:

        mov ah, 0x01                 ; (get status of last drive operation)
        shelve_values_from_protected_mode 1, eax

        shelve_protected_mode_with_entry_address_fpm _low_disk_read_sectors_i2
        execute_real_mode_0x13

    _low_disk_read_sectors_i2:
        unshelve_values_from_protected_mode 1, eax

        mov al, ah                   ; (result status code, 0 means no error)
        and eax, 0xFF

        mov esp, ebp
        pop ebp
        ret
