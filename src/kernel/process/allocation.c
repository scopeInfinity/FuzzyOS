#include <fuzzy/kernel/process/process.h>
#include <fuzzy/memmgr/tables/gdt.h>
#include <fuzzy/memmgr/layout.h>
#include <fuzzy/kernel/panic.h>

#include <process.h>

/* Process ID
 *  pid 0 : kernel core
 *  pid 1 : user app 1
 *  pid 2 : user app 2 and so on...
 *
 * Process Cycle
 *  - process_create(...)
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

// operations
static struct Process processes[MAX_PROCESS] = {0};
struct GDTEntry gdt_table[GDT_TABLE_SIZE];
struct GDTReference gdtr;

struct Process *get_process(int pid) {
    if(pid<0 || pid>=MAX_PROCESS) return NULL;
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
    processes[PID_KERNEL].ppid=PID_KERNEL;

    populate_gdt_table(
        &gdtr, gdt_table, GDT_TABLE_SIZE,
        MEMORY_KERNEL_LOCATION);
    load_gdt_table(&gdtr);
}

// return new user_sp
static int create_infant_process_argv_stack(int user_ds, int user_sp,
        int argc, char *argv[]) {
    // assumes pointer are 4 bytes and are same as int

    user_sp = user_sp - sizeof(ARGV);
    char *__us_argv_data = user_sp;
    syscall_strncpy_kernel_to_user(user_ds, __us_argv_data, argv, sizeof(ARGV));

    // assumes argc < PROCESS_MAX_ARGC
    char *ks_to_us_argv[PROCESS_MAX_ARGC] = {NULL};
    for (int i = 0; i < argc; i++) {
        ks_to_us_argv[i] = __us_argv_data + i*PROCESS_MAX_ARG_LEN;
    }


    user_sp = user_sp - sizeof(ks_to_us_argv);
    char *__us_argv_list = user_sp;
    syscall_strncpy_kernel_to_user(user_ds, __us_argv_list, ks_to_us_argv, sizeof(ks_to_us_argv));

    user_sp = user_sp - sizeof(argc);
    char *__us_argv = user_sp;
    syscall_strncpy_kernel_to_user(user_ds, __us_argv, &__us_argv_list, sizeof(__us_argv_list));

    user_sp = user_sp - sizeof(argc);
    char *__us_argc = user_sp;
    syscall_strncpy_kernel_to_user(user_ds, __us_argc, &argc, sizeof(argc));

    return user_sp;
}

static int get_cold_pid_to_allocate() {
    static int pid = 0;
    pid = 0;  // BUG: What PID wrap around doesn't work?
    for (int i = 0; i < MAX_PROCESS; ++i) {
        pid = (pid+1)%MAX_PROCESS;
        if (pid!=PID_KERNEL && processes[pid].state == STATE_COLD) {
            // return a PID to allocate
            return pid;
        }
    }
    // no free PID found.
    return -1;
}

int process_create(unsigned int ppid, int argc, char *argv[]) {
    // returnd pid >= 0 if success
    int pid = get_cold_pid_to_allocate();
    if(pid < 0) return pid;
    struct Process *process = &processes[pid];
    // reset state
    process->flagirq0_fork_ready = 0;
    process->flagirq0_fork_newchild = -1;

    int memory_location = memmgr_app_abs_location(pid);
    int memory_size = memmgr_app_size(pid);
    int idt_cs_entry = get_idt_cs_entry(pid);
    int idt_ds_entry = get_idt_ds_entry(pid);

    // Potential improvement:
    // - avoid populating GDT entry if already exists.

    // Application Code Segment Selector
    populate_gdt_entry(
        &gdt_table[idt_cs_entry],
        memory_location, memory_location+memory_size-1,
        0b0100,  // 32-bit protected mode
        0x9a);
    // Application Data Segment Selector
    populate_gdt_entry(
        &gdt_table[idt_ds_entry],
        memory_location, memory_location+memory_size-1,
        0b0100,  // 32-bit protected mode
        0x92);


    // update process state
    process->ppid = ppid;
    process->state = STATE_LOADING;
    process->cs = get_gdt_number_from_entry_id(idt_cs_entry);
    process->ip = 0;
    // initially ds == ss
    process->ss = get_gdt_number_from_entry_id(idt_ds_entry);
    if (argc > 0) {
        // process_create for those who don't care about stack initilization
        // like fork(), they will copy whole segment memory.
        process->sp = create_infant_process_argv_stack(process->ss, STACKINIT_APP, argc, argv);
        process->sp = create_infant_process_irq0_stack(process->ss, process->sp);
    }
    return pid;
}

void process_kill(unsigned int pid, int status) {
    struct Process *process = &processes[pid];
    process->status_code = status;
    process->state = STATE_EXIT;
}

int process_fork_mark_ready(int pid) {
    struct Process *process = get_process(pid);
    if(process == NULL) return -2;
    if(process->flagirq0_fork_ready>0) return -3;  // denied; fork already requested.
    process->flagirq0_fork_ready = 1;   // success; fork requested
    return 0;
}

int process_fork_check_ready(int pid) {
    // should be called after process_fork_mark_ready
    struct Process *process = get_process(pid);
    if(process == NULL) return -2;
    if(process->flagirq0_fork_ready==-1) return -3;  // fork request: failed
    // bad state from user side
    if(process->flagirq0_fork_ready==1) return -4;    // fork request: still waiting
    return process->flagirq0_fork_newchild;  // fork request: success and return new pid
}

int process_fork(unsigned int ppid) {
    // should be called from irq0 only.
    int npid = process_create(ppid, 0, NULL);
    if(npid < 0) return npid;
    struct Process *process = get_process(ppid);
    struct Process *nprocess = get_process(npid);

    nprocess->state = STATE_READY;

    nprocess->sp = process->sp;
    nprocess->ip = process->ip;

    // HERE
    // process->state = STATE_EXIT;

    size_t size = min(
        memmgr_app_size(ppid),
        memmgr_app_size(npid)
    );
    int dst_ds = get_gdt_number_from_entry_id(get_idt_ds_entry(npid));
    int src_ds = get_gdt_number_from_entry_id(get_idt_ds_entry(ppid));

    kernel_memncpy_absolute(
        dst_ds,
        0,
        src_ds,
        0,
        size
    );
    // print_log("copy success");
    // PANIC(0, "A");
    return npid;
}

int process_load_from_disk(int pid, int lba_index, int sector_count) {
    int memory_location = memmgr_app_abs_location(pid);
    int err = load_sectors(memory_location, 0x80, lba_index, sector_count);
    if(err) {
        return err;
    }
    return 0;
}
