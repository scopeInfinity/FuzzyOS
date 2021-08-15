#include <fuzzy/kernel/process/process.h>
#include <fuzzy/memmgr/tables/gdt.h>

#include <fuzzy/kernel/panic.h>

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
    return (process_id<<1)+3;
}

int get_idt_ds_entry(int process_id) {
     if(process_id == PID_KERNEL) {
        return 2;
    }
   return (process_id<<1)+4;
}

int get_gdt_number_from_entry_id(int id) {
    return id*sizeof(struct GDTEntry);
}

int get_idt_reverse_pid_lookup_cs(int cs) {
    if(cs%sizeof(struct GDTEntry)!=0) {
        PANIC(cs, "get_idt_reverse_pid_lookup(cs) called with cs%8!=0.");
    }
    int segment_id = cs/sizeof(struct GDTEntry);
    if(segment_id<=0) {
        PANIC(cs, "get_idt_reverse_pid_lookup(cs) called with segment_id<=0.");
    }
    int cs_index = (segment_id-1)/2;
    if(cs_index==0) {
        return PID_KERNEL;
    }
    // cs_index == 1 is absolute segment gdt entry
    return cs_index-1;
}

int get_idt_reverse_pid_lookup_ds(int ds) {
    if(ds%sizeof(struct GDTEntry)!=0) {
        PANIC(ds, "get_idt_reverse_pid_lookup(ds) called with ds%8!=0.");
    }
    int segment_id = ds/sizeof(struct GDTEntry);
    if(segment_id<0) {
        PANIC(ds, "get_idt_reverse_pid_lookup(ds) called with segment_id<2.");
    }
    int ds_index = segment_id/2-1;
    if(ds_index==0) {
        return PID_KERNEL;
    }
    // ds_index == 1 is absolute segment gdt entry
    return ds_index-1;
}

// memory mapping
int get_process_memory(int pid) {
    if(pid == PID_KERNEL) {
        return MEMORY_LOCATION_KERNEL;
    }
    return MEMORY_LOCATION_APP+0x20000*(pid-1);
}

// operations
static struct Process processes[MAX_PROCESS] = {0};
struct GDTEntry gdt_table[GDT_TABLE_SIZE];
struct GDTReference gdtr;

struct Process *get_process(int pid) {
    return &processes[pid];
}

void process_scheduler_init() {
    print_log("Process scheduler init");
    for (int i = 0; i < MAX_PROCESS; ++i) {
        processes[i].state=STATE_COLD;
    }
    // after the process scheduler starts
    // main thread kernel is ready to be killed.
    processes[PID_KERNEL].state=STATE_EXIT;

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
    struct Process *process = &processes[pid];

    int memory_location = get_process_memory(pid);
    int idt_cs_entry = get_idt_cs_entry(pid);
    int idt_ds_entry = get_idt_ds_entry(pid);

    process->state = STATE_LOADING;
    process->cs = get_gdt_number_from_entry_id(idt_cs_entry);
    process->ip = 0; //
    // initially ds == ss
    process->ss = get_gdt_number_from_entry_id(idt_ds_entry);
    // should be compatible with create_infant_process_irq0_stack
    process->sp = 0xFFF0-56;  // keep offset in sync with _int_irq0_start


    // Potential improvement:
    // - avoid populating GDT entry if already exists.

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
    create_infant_process_irq0_stack(process->ss);
    return pid;
}

void process_kill(int user_ds, int status) {
    int pid = get_idt_reverse_pid_lookup_ds(user_ds);
    struct Process *process = &processes[pid];
    process->status_code = status;
    process->state = STATE_EXIT;
}

int process_load_from_disk(int pid, int lba_index, int sector_count) {
    int memory_location = get_process_memory(pid);
    int err = load_sectors(memory_location, 0x80, lba_index, sector_count);
    if(err) {
        return err;
    }
    return 0;
}
