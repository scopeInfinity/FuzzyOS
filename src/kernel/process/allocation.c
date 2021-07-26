#include <fuzzy/memmgr/tables/gdt.h>
#include <fuzzy/kernel/process/process.h>
#include <lib/utils/panic.h>

#include "memmgr/tables/gdt.c"

#define PID_KERNEL 0

/* Process ID
 *  pid 0 : kernel core
 *  pid 1 : user app 1
 *  pid 2 : user app 2 and so on...
 *
 * Process Cycle
 *  - process_create()
 *  - process_load_from_disk()
 *  - OR process_load_from_ram()
 */

int get_idt_cs_entry(int process_id) {
    if(process_id == PID_KERNEL) {
        return 1;
    }
    return (process_id<<1)+5;
}

int get_idt_ds_entry(int process_id) {
     if(process_id == PID_KERNEL) {
        return 2;
    }
   return (process_id<<1)+6;
}

int get_idt_reverse_pid_lookup(int cs) {
    if(cs%sizeof(struct GDTEntry)!=0) {
        PANIC(cs, "get_idt_reverse_pid_lookup(cs) called with cs%8!=0.");
    }
    if(cs<=0) {
        PANIC(cs, "get_idt_reverse_pid_lookup(cs) called with cs<=0.");
    }
    int segment_id = cs/sizeof(struct GDTEntry);
    int idt_ss_entry = (segment_id-1)/2;
    if(idt_ss_entry==0) {
        return PID_KERNEL;
    } else if(idt_ss_entry==-1) {
        // TODO: Maybe add process manager as separate process.
        return -1; // process manager
    }
    return (idt_ss_entry-6)/2;
}

// memory mapping
int get_process_memory(int pid) {
    if(pid == PID_KERNEL) {
        return MEMORY_LOCATION_KERNEL;
    }
    return MEMORY_LOCATION_APP+0x20000*pid;
}

// operations
static struct Process processes[MAX_PROCESS] = {0};
struct GDTEntry gdt_table[GDT_TABLE_SIZE];
struct GDTReference gdtr;

struct Process *process_get(int pid) {
    return &processes[pid];
}

void process_scheduler_init() {
    print_log("Process scheduler init");
    for (int i = 0; i < MAX_PROCESS; ++i) {
        processes[i].state=STATE_COLD;
    }
    populate_gdt_table(
        &gdtr, gdt_table, GDT_TABLE_SIZE,
        MEMORY_LOCATION_KERNEL);
    load_gdt_table(&gdtr);
}

int process_create() {
    // returnd pid >= 0 if success
    int pid = -1;
    for (int i = 0; i < MAX_PROCESS; ++i) {
        if(i==PID_KERNEL) continue;
        if(processes[i].state == STATE_COLD) {
            pid = i;
            break;
        }
    }
    if(pid < 0) return pid;
    processes[pid].state = STATE_LOADING;

    int memory_location = get_process_memory(pid);
    int idt_cs_entry = get_idt_cs_entry(pid);
    int idt_ds_entry = get_idt_ds_entry(pid);

    // TODO: avoid populating GDT entry if already exists.
    // Application Code Segment Selector
    populate_gdt_entry(
        &gdt_table[idt_cs_entry],
        memory_location, memory_location+0xFFFF,
        0b0100,  // 32-bit protected mode
        0x9a);
    // Application Data Segment Selector
    populate_gdt_entry(
        &gdt_table[idt_ds_entry],
        memory_location, memory_location+0xFFFF,
        0b0100,  // 32-bit protected mode
        0x92);
    return -1;
}

int process_load_from_disk(int pid, int lba_index, int sector_count) {
    int memory_location = get_process_memory(pid);
    int err = load_sectors(memory_location, 0x80, lba_index, sector_count);
    if(err) {
        print_log("failed to load app %d in memory, Error: ", pid, err);
        return -1;
    }
    // TODO(scopeinfinity): fix launch registers.
    processes[pid].state = STATE_READY;
    return 0;
}
