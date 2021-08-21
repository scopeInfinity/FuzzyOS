; constants only

; Kernel core is app with pid 0
; Kernel event is trigged via IRQ0 or syscall,

STACKINIT_APP EQU 0xFFF0
STACKINIT_KERNEL_CORE EQU STACKINIT_APP
STACKINIT_KERNEL_EVENT EQU 0xC000

