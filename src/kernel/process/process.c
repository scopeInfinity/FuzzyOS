#include <fuzzy/kernel/process/process.h>
#include <fuzzy/kernel/interrupts/timer.h>

#include <process.h>

#include <lib/utils/logging.h>
#include <lib/utils/output.h>


// int process_free_id(int id) {
//     process_availability[id] = 1;
// }

int process_new_allocated_memory(int id) {
    return MEMORY_LOCATION_APP+0x10000*id;
}

extern int call_main(int cs, int ds, int argc, char *argv[]);

/*
TODO: fix documentation.
User side memory view
CS 0x0000 -> ...
DS 0x0000 -> 0xFFFF
FS ; same as DS
GS ; same as DS
SS ...    <- 0xDFFF  ; user stack
SS 0xE000 <- 0xFFFF  ; kernel stack
*/
int process_spawn(int lba_index, int sector_count) {
    print_info("[process_spawn] create");
    int pid = process_create();
    if(pid<0) {
        print_log("Failed to reserved a new pid");
        return -1;
    }
    print_info("[process_spawn] loading, pid: %d", pid);
    int err = process_load_from_disk(pid, lba_index, sector_count);
    if (err) {
        print_log("Failed to load app in memory, Error: ", err);
        return -2;
    }
    print_info("[process_spawn] ready, pid: %d", pid);
    struct Process *process = get_process(pid);
    // TODO(scopeinfinity): Uncomment when create_infant_process_irq0_stack is ready.
    process->state = STATE_READY;
    // TODO(scopeinfinity): Remove call_main once process_scheduler starts working.
    // call_main(process->cs, process->ss, 0, 0);
    return 0;
}

int process_exec(int lba_index, int sector_count) {
    // Not yet implemented
    return -1;
}

int syscall_1_process_spawn_lba_sc(int lba_start, int sector_count) {
    return process_spawn(lba_start, sector_count);
}

int syscall_1_process_exec_lba_sc(int lba_start, int sector_count) {
    return process_exec(lba_start, sector_count);
}

int syscall_1_process(int operation, int a0, int a1, int a2, int a3, int user_ds) {
    switch (operation) {
        case SYSCALL_PROCESS_SUB_SPAWN_LBA_SC:
            return syscall_1_process_spawn_lba_sc(a0, a1);
        case SYSCALL_PROCESS_SUB_EXEC_LBA_SC:
            return syscall_1_process_exec_lba_sc(a0, a1);
    }
    return -1;
}
