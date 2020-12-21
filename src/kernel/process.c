#pragma once
#include <lib/utils/logging.h>
#include <lib/utils/output.h>

#include "memmgr/tables/gdt.c"

#define GDT_TABLE_SIZE 25
struct GDTEntry gdt_table[GDT_TABLE_SIZE]={0};
struct GDTReference gdtr={0};

#define MAX_PROCESS 10
struct Process processes[MAX_PROCESS] = {0};
// int process_availability[MAX_PROCESS]={0};

struct Process {
    int state;
    int cs;
    int ds;

    int ip;
    int sp;
    int bp;

    int exit_code;
};

#define PROCESS_STATE_COLD     0
#define PROCESS_STATE_READY    1
#define PROCESS_STATE_RUNNING  2

inline Process* process_get(int id) {
    return &processes[id];
}

void process_free(id) {
    Process *process = process_get(id);
    process->state = PROCESS_STATE_COLD;
}

int process_create(int sector_index, int sector_count) {
    int id = process_reserve_new_id();
    Process *process = process_get(id);
    process->state = PROCESS_STATE_COLD;
    process->id = id;
    if (process->id < 0) {
        print_log("Failed to reserved a new process ID");
        return -1;
    }
    int memory_location = _process_new_allocated_memory(process->id);

    print_log("[process_%d] Creating process with sector: %d count: %d",
        process->id, sector_index, sector_count);

    int err = load_sectors(memory_location, 0x80, sector_index, sector_count);
    if(err) {
        print_log("[process_%d] Failed to load app in memory, Error: ", err);
        return -1;
    }
    int idt_cs_entry = (id<<1)+5;
    int idt_ds_entry = (id<<1)+6;

    // Application Code Segment Selector
    populate_gct_entry(
        &gdt_table[idt_cs_entry],
        memory_location, memory_location+MEMORY_LOCATION_APP_ESIZE-1,
        0b0100,  // 32-bit protected mode
        0x9a);
    // Application Data Segment Selector
    populate_gct_entry(
        &gdt_table[idt_ds_entry],
        memory_location, memory_location+MEMORY_LOCATION_APP_ESIZE-1,
        0b0100,  // 32-bit protected mode
        0x92);

    int relative_address = memory_location-MEMORY_LOCATION_KERNEL;
    process->cs = idt_cs_entry*sizeof(struct GDTEntry);
    process->ds = idt_ds_entry*sizeof(struct GDTEntry);

    process->ip = 0;
    process->sp = MEMORY_LOCATION_APP_ESIZE-4;

    print_log("[process_%d] App loaded at 0x%x, relative_address: 0x%x: %x...",
        memory_location, relative_address,
        *(int*)relative_address);
    process->state = PROCESS_STATE_READY;
    return id;
}

extern int _low_process_jmp_running(int cs, int ds, int ip, int sp, int bp);
void process_force_move_to_running(id) {
    Process *process = process_get(id);
    process->state = PROCESS_STATE_RUNNING;
    _low_process_jmp_running(
        process->cs, process->ds,
        process->ip, process->sp, process->bp);
    process->state = PROCESS_STATE_EXIT;
}

void process_wait_for_completion(int id) {
    Process *process = process_get(id);
    while (process->state != PROCESS_STATE_EXIT);
    process_free(id);
}

void _process_register_kernel() {
    Process *kernel = process_get(0);
    kernel->state = PROCESS_STATE_RUNNING;
    process->cs = GDT_ENTRY_KERNEL_CS*sizeof(struct GDTEntry);
    process->ds = GDT_ENTRY_KERNEL_DS*sizeof(struct GDTEntry);
}

void process_handler_init() {
    print_log("Process handler init");
    for (int i = 0; i < MAX_PROCESS; ++i) {
        process_free(i);
    }
    _process_register_kernel();
    populate_gdt_table(
        &gdtr, gdt_table, GDT_TABLE_SIZE,
        MEMORY_LOCATION_KERNEL);
    load_gdt_table(&gdtr);
}

int _process_reserve_new_id() {
    // id: application id, 0-based
    for (int i = 0; i < MAX_PROCESS; ++i) {
        Process *process = process_get(i);
        if (process->state == PROCESS_STATE_COLD) {
            return i;
        }
    }
    return -1;
}

int _process_new_allocated_memory(int id) {
    return MEMORY_LOCATION_APP_START+MEMORY_LOCATION_APP_ESIZE*id;
}

extern int call_main(int cs, int ds, int argc, char *argv[]);

int process_exec(int sector_index, int sector_count) {
    int id = process_create(sector_index, sector_count);
    Process *process = process_get(id);
    int argc = 0;
    int argv = 0;
    int code_segment = process->cs;
    int data_segment = process->ds;

    print_log("call_main(0x%x:0, 0x%x:0, %d, %d)", code_segment, data_segment, argc, argv);
    // Expect some one to schedule the process.
    process_force_move_to_running(id);
    process_wait_for_completion(id);
    io_low_flush();
    return process->exit_code;
}

