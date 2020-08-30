%macro  PROCESS_MACRO_MANAGER 0

global process_manager_esp
global process_esp
global process_ss

[SECTION .data]
    process_manager_esp  dd 0
    process_esp          dd 0
    process_ss           dw 0

%endmacro

%macro  PROCESS_MACRO_USER 0

extern process_esp
extern process_ss

%endmacro

%macro  PROCESS_SHELVE 0
        ; Using process_unshelve as label instead
        ; of function to avoid use of stack.
        ; int should have pushed cs:ip on stack.
        ; TODO: Fix pushfl complains.
        pushfd
        pushad
        xor eax, eax
        mov ax, ds
        push eax  ; push ds

        mov ax, 0x10
        mov es, ax
        mov ds, ax
        mov fs, ax
        mov gs, ax
        ; Save process ESP and
        ; load progress manager SS:ESP.
        mov ebx, esp
        mov [process_esp], ebx
        mov bx, ss
        mov [process_ss], bx

        mov ax, 0x30    ; Process Manager SS
        mov ss, ax
        mov ebx, 0xFFFC ; [process_manager_esp]
        mov esp, ebx
%endmacro

%macro  PROCESS_UNSHELVE 0
        ; Using process_unshelve as label instead
        ; of function to avoid use of stack.
        mov eax, [process_esp]
        mov esp, eax
        mov bx, [process_ss]
        mov ss, bx
        mov [0x4600], bx

        ; Expects process SS:ESP to be loaded.
        pop eax  ; pop ds
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        popad
        popfd
        ; iret can pop cs:ip appropriately.
%endmacro

%macro PROCESS_MACRO_PREPARE_NEW 0
        ; returns user program stack
        push ebp
        mov ebp, esp
        mov dx, ss  ; saving ss and esp temporarily.

        ; entering user process stack temporarily.
        mov ecx, [ebp + 0x08]         ; (cs)
        mov ebx, [ebp + 0x0c]         ; (ds)
        mov eax, [ebp + 0x10]         ; (ss)

        mov esp, 0xFFF0  ; keeping some buffer for extras.
        mov ss, ax

        push ecx     ; similar to push cs within int $x
        xor ecx, ecx
        push ecx    ; push IP as 0

        pushfd
        pushad
        push ebx  ; equivalent to push ds

        mov eax, esp  ; Top of user program stack

        mov ss, dx   ; restoring ss
        mov esp, ebp ; restoring esp
        pop ebp
        ret
%endmacro
