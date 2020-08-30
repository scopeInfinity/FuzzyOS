%macro  PROCESS_MACRO_MANAGER 0

global process_manager_esp
global process_esp
global process_ss

global process_eax
global process_ebx
global process_ecx
global process_edx
global process_esi
global process_edi

[SECTION .data]
    process_manager_esp  dd 0
; Following values gets INT locked and only one ISR can use them
; at a time.
    process_esp          dd 0
    process_ss           dw 0
    process_eax          dd 0
    process_ebx          dd 0
    process_ecx          dd 0
    process_edx          dd 0
    process_esi          dd 0
    process_edi          dd 0
%endmacro

%macro  PROCESS_MACRO_USER 0

; High Level process shelver and unshelver
extern process_shelve_context
extern process_unshelve_context

extern process_esp
extern process_ss

extern process_eax
extern process_ebx
extern process_ecx
extern process_edx
extern process_esi
extern process_edi

%endmacro

%macro  PROCESS_SHELVE 1
        ; Args: (is_blocking)
        CLI
        ; TODO: Fix as this may not work under nested conditions.
        ; Using process_unshelve as label instead
        ; of function to avoid use of stack.
        ; int should have pushed cs:ip on stack.
        ; This should preserve the value of eax, ebx, ecx, edx, esi, edi

        ; pushad will get poped before end of macro list it was never pushed
        pushad

        xor eax, eax
        mov ax, ds
        push eax  ; push ds

        mov ax, 0x10
        mov es, ax
        mov ds, ax
        mov fs, ax
        mov gs, ax

        ; Move stack past DS and retrieve register value from stack.
        ; Temporarily stash general purpose register in memory.
        add esp, 4
        popad
        pushad
        sub esp, 4
        mov [process_eax], eax
        mov [process_ebx], ebx
        mov [process_ecx], ecx
        mov [process_edx], edx
        mov [process_esi], esi
        mov [process_edi], edi
        pop eax      ; temporarily get back ds
        add esp, 32  ; delete pushad from stack
        push eax     ; push ds back to stack.

        ; Save process ESP and
        mov ebx, esp
        mov [process_esp], ebx
        mov bx, ss
        mov [process_ss], bx
        ; TODO: Why are we using process manager stack for SYSCALL?
        ; load progress manager SS:ESP.
        mov ax, 0x30    ; Process Manager SS
        mov ss, ax
        mov ebx, 0xFFFC ; [process_manager_esp]
        mov esp, ebx

        push %1
        call process_shelve_context
        add esp, 4

        ; load original general purpose register value from memory.
        mov eax, [process_eax]
        mov ebx, [process_ebx]
        mov ecx, [process_ecx]
        mov edx, [process_edx]
        mov esi, [process_esi]
        mov edi, [process_edi]
        STI
%endmacro

%macro  PROCESS_UNSHELVE 0
        ; Using process_unshelve as label instead
        ; of function to avoid use of stack.
        ; This should preserve the value of eax
        CLI
        mov [process_eax], eax
        call process_unshelve_context

        mov eax, [process_esp]
        mov esp, eax
        mov bx, [process_ss]
        mov ss, bx

        ; Expects process SS:ESP to be loaded.
        pop ebx  ; pop ds
        mov es, bx
        mov fs, bx
        mov gs, bx
        mov eax, [process_eax]
        mov ds, bx
        ; iret can pop cs:ip appropriately.
        STI
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