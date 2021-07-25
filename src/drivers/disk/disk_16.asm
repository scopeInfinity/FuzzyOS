[BITS 16]

global _low_disk_read_sectors

[SECTION .text]
    _low_disk_read_sectors:
        push ebp
        mov ebp, esp
        ; callee save register
        push ebx
        push esi
        push edi

        ; check es
        mov ah, 0x02                ; (read sectors)
        mov al, [ebp + 0x8]         ; (sector count)
        mov cx, [ebp + 0xc]         ; (cylinder 10bit, sector 6bit)
        mov dx, [ebp + 0x10]        ; (head, drive index)
        mov bx, [ebp + 0x14]        ; (es:bx as write address)
        int 0x13

        mov ah, 0x01                 ; (get status of last drive operation)
        int 0x13
        mov al, ah                   ; (result status code, 0 means no error)
        and eax, 0xFF

        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
