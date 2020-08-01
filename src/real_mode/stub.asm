; 0x7F00 : 6  bytes   ; GDTR Address
; 0x7F08 : 4  bytes   ; Protected mode entry point
; 0x7F0C : 8  bytes   ; Protected mode shelved ebp and esp registers
; 0x7F04 : 12 bytes   ; FREE
; 0x7F64 : -          ; Shelved data values
; Notes:
; - Defined macros tries to avoid dependency on stack while in real mode.

; Should be called from real mode only.
%macro  set_protected_mode_entry_address_frm 1
        ; Args: (entry_point)
        ; Note: entry_point as 0 is magic address for kernel init.
        mov eax, %1
        mov [0x7F08], eax   ; set protected mode next jump to given address
%endmacro

; Should be called from protected mode only.
%macro  shelve_protected_mode_with_entry_address_fpm 1
        ; Args: (entry_point)
        ; Note: entry_point as 0 is magic address for kernel init.

        mov cx, ds
        mov ax, 0x20    ; Absolute Data Segment Selector
        mov ds, ax

        mov ebx, ebp
        mov [0x7F0C], ebx ; Save ebp

        mov ebx, esp
        mov [0x7F10], ebx ; Save esp

        mov ebx, %1
        mov [0x7F08], ebx ; Save entry point

        mov ds, cx
%endmacro

; Should be called from protected mode only.
%macro  unshelve_protected_mode_and_ret_entry_address 0
        ; Out: (eax: entry_address)
        mov cx, ds
        mov ax, 0x20    ; Absolute Data Segment Selector
        mov ds, ax

        mov ebx, [0x7F0C] ; Get ebp
        mov ebp, ebx

        mov ebx, [0x7F10] ; Get esp
        mov esp, ebx

        mov eax, [0x7F08] ; Get entry point

        mov ds, cx
%endmacro

; Should be called from protected mode only.
%macro  shelve_values_from_protected_mode 2
        ; Args: (index, in register)
        push %2

        mov cx, ds
        mov ax, 0x20    ; Absolute Data Segment Selector
        mov ds, ax

        pop ebx
        mov [0x7F64+(%1)*4], ebx

        mov ds, cx
%endmacro

; Should be called from protected mode only.
%macro  unshelve_values_from_protected_mode 2
        ; Args: (index, return register)
        mov cx, ds
        mov ax, 0x20    ; Absolute Data Segment Selector
        mov ds, ax

        mov ebx, [0x7F64+(%1)*4]
        push ebx

        mov ds, cx

        pop %2
%endmacro

; Should be called from real mode only.
%macro  shelve_values_from_real_mode 2
        ; Args: (index, in register)
        mov [0x7F64+(%1)*4], %2
%endmacro

; Should be called from real mode only.
%macro  unshelve_values_from_real_mode 2
        ; Args: (index, return register)
        mov %2, [0x7F64+(%1)*4]
%endmacro

; Should be called from protected mode only.
%macro  get_protected_mode_entry 0
        ; Out: (eax)
        ; Note: entry_point as 0 is magic address for kernel init.
        mov cx, ds
        mov ax, 0x20    ; Absolute Data Segment Selector
        mov ds, ax
        mov eax, [0x7F08]
        mov ds, cx
%endmacro

; Should be called from real mode only.
; 6 bytes
%macro  get_gdtr_address 0
        mov eax, 0x7F00
%endmacro

; Should be called from protected mode only.
; The protected mode return address should be saved using above macro  beforehand.
%macro  execute_real_mode_0x13 0
        jmp 0x18:0x7E00
%endmacro