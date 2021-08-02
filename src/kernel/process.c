#pragma once
#include <lib/utils/logging.h>
#include <lib/utils/output.h>

#include "memmgr/tables/gdt.c"

#define GDT_TABLE_SIZE 25
struct GDTEntry gdt_table[GDT_TABLE_SIZE];
struct GDTReference gdtr;

#define MAX_PROCESS 10
int process_availability[MAX_PROCESS];

void process_handler_init() {
    print_log("Process handler init");
    for (int i = 0; i < MAX_PROCESS; ++i) {
        process_availability[i]=1;
    }
    populate_gdt_table(
        &gdtr, gdt_table, GDT_TABLE_SIZE,
        MEMORY_LOCATION_KERNEL);
    load_gdt_table(&gdtr);
}

int process_reserve_new_id() {
    // Only one process can at a time for now.
    // id: application id, 0-based
    for (int i = 0; i < MAX_PROCESS; ++i) {
        if(process_availability[i]) {
            process_availability[i] = 0;
            return i;
        }
    }
    return -1;
}

int process_free_id(int id) {
    process_availability[id] = 1;
}

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
int process_exec(int lba_index, int sector_count) {
    int id = process_reserve_new_id();
    if(id<0) {
        print_log("Failed to reserved a new process ID");
        return -1;
    }
    print_log("[process] pid:%d, lba: %d, sector_count: %d",
        id, lba_index, sector_count);
    int memory_location = process_new_allocated_memory(id);

    int err = load_sectors(memory_location, 0x80, lba_index, sector_count);
    if(err) {
        print_log("Failed to load app in memory, Error: ", err);
        return -1;
    }
    int idt_cs_entry = (id<<1)+5;
    int idt_ds_entry = (id<<1)+6;
    // Application Code Segment Selector
    populate_gct_entry(
        &gdt_table[idt_cs_entry],
        memory_location, memory_location+0xFFFF,
        0b0100,  // 32-bit protected mode
        0x9a);
    // Application Data Segment Selector
    populate_gct_entry(
        &gdt_table[idt_ds_entry],
        memory_location, memory_location+0xFFFF,
        0b0100,  // 32-bit protected mode
        0x92);

    int relative_address = memory_location-MEMORY_LOCATION_KERNEL;
    print_log("App loaded at 0x%x, relative_address: 0x%x: %x...",
        memory_location, relative_address,
        *(int*)relative_address);

    int code_segment = idt_cs_entry*sizeof(struct GDTEntry);
    int data_segment = idt_ds_entry*sizeof(struct GDTEntry);
    int argc = 0;
    int argv = 0;
    print_log("call_main(0x%x, 0x%x, %d, %d)", code_segment, data_segment, argc, argv);

    int exit_code = call_main(code_segment, data_segment, argc, argv);

    process_free_id(id);
    io_low_flush();
    return exit_code;
}
