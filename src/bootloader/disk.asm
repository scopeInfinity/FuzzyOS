; Not: sector is 1 based indexing here.
%macro  disk_read 6
        ; Args: (sector_count, drive 8bit, cylinder 10bit, head 8bit, sector 6bit, write_add)
        ; check es
        mov ax, 0x02%1              ; (read sectors, sector count)
        mov cx, 0x%3%5              ; (cylinder 10bit, sector 6bit)
        mov dx, 0x%4%2              ; (head, drive index)
        mov bx, 0x%6                ; (es:bx as write address)
        int 0x13
%endmacro

%macro  disk_write 6
        ; Args: (sector_count, drive 8bit, cylinder 10bit, head 8bit, sector 6bit, read_add)
        ; check es
        mov ax, 0x03%1              ; (write sectors, sector count)
        mov cx, 0x%3%5              ; (cylinder 10bit, sector 6bit)
        mov dx, 0x%4%2              ; (head, drive index)
        mov bx, 0x%6                ; (es:bx as write address)
        int 0x13
%endmacro

%macro  disk_success 1
        ; Args: (drive index 8 bit)
        ; Result: (AH: status code, set CF on error)
        mov ah, 0x01                ; (get status of last drive operation)
        mov dl, 0x%1                ; (drive index)
        int 0x13
%endmacro