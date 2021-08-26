#pragma once

// START_ENSURE_SAME_layout_asm
#define MEMORY_APP_SIZE         0x20000
#define STACKINIT_APP           (MEMORY_APP_SIZE-4)
// END_ENSURE_SAME_layout_asm

// Most of the memory layout relies on the fact the kernel is first app
// and kernel's pid is 0.
#define PID_KERNEL 0

#define MEMORY_APPBASE_LOCATION 0x10000

#define memmgr_app_abs_location(pid) (MEMORY_APPBASE_LOCATION + (pid)*MEMORY_APP_SIZE)
#define memmgr_app_size(pid) (MEMORY_APP_SIZE)

#define MEMORY_KERNEL_LOCATION memmgr_app_abs_location(PID_KERNEL)
#define MEMORY_KERNEL_SIZE memmgr_app_size(PID_KERNEL)
