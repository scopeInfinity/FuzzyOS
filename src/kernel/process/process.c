#include <fuzzy/kernel/process/process.h>
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
    // int id = process_reserve_new_id();
    // if(id<0) {
    //     print_log("Failed to reserved a new process ID");
    //     return -1;
    // }
    // print_log("[process] pid:%d, lba: %d, sector_count: %d",
    //     id, lba_index, sector_count);
    // int memory_location = process_new_allocated_memory(id);

    // int err = load_sectors(memory_location, 0x80, lba_index, sector_count);
    // if(err) {
    //     print_log("Failed to load app in memory, Error: ", err);
    //     return -1;
    // }

    // int relative_address = memory_location-MEMORY_LOCATION_KERNEL;
    // print_log("App loaded at 0x%x, relative_address: 0x%x: %x...",
    //     memory_location, relative_address,
    //     *(int*)relative_address);

    // int code_segment = idt_cs_entry*sizeof(struct GDTEntry);
    // int data_segment = idt_ds_entry*sizeof(struct GDTEntry);
    // int argc = 0;
    // int argv = 0;
    // print_log("call_main(0x%x, 0x%x, %d, %d)", code_segment, data_segment, argc, argv);

    // int exit_code = call_main(code_segment, data_segment, argc, argv);

    // process_free_id(id);
    // io_low_flush();
    // return exit_code;
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
