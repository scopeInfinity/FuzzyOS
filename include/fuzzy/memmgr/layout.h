#pragma once

// Most of the memory layout relies on the fact the kernel is first app
// and kernel's pid is 0.
#define PID_KERNEL 0

#define MEMORY_KERNEL_LOCATION 0xC000
#define MEMORY_KERNEL_SIZE     0xFFFF

#define MEMORY_APPBASE_LOCATION 0x20000
#define MEMORY_APP_SIZE         0x0FFFF

// pid starts from 1
#define memmgr_app_abs_location(pid) ((pid)!=0?(MEMORY_APPBASE_LOCATION+((pid)-1)*MEMORY_APP_SIZE):MEMORY_KERNEL_LOCATION)
#define memmgr_app_size(pid) (MEMORY_APP_SIZE)
