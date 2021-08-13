#include <fuzzy/kernel/process/process.h>

int process_scheduler_rr(int lastpid) {
    // Round Robin Scheduler.
    int best_id = lastpid;
    for (int i = 1; i <= MAX_PROCESS; ++i) {
        int nid = (lastpid+i)%MAX_PROCESS;
        struct Process *process = get_process(nid);
        if (process->state == STATE_READY) {
            best_id = nid;
            break;
        }
    }
    return best_id;
}

int process_scheduler_largestpid(int lastpid) {
    // Run largest READY pid
    int best_id = lastpid;
    for (int nid = MAX_PROCESS-1; nid >= 0; --nid) {
        struct Process *process = get_process(nid);
        if (process->state == STATE_READY) {
            best_id = nid;
            break;
        }
    }
    return best_id;
}

int process_scheduler_get_next_pid(int lastpid) {
    // execute one of the scheduling algorithm
    return process_scheduler_largestpid(lastpid);
}

void process_scheduler(int *_e_ip, int *_e_cs, int *_e_sp, int *_e_ss) {
    int e_ip = *_e_ip;
    int e_cs = *_e_cs;
    int e_sp = *_e_sp;
    int e_ss = *_e_ss;
    print_info("[process_scheduler] cs:ip %x:%x,  ss:sp %x:%x",
        e_cs, e_ip,
        e_ss, e_sp);
    // TODO: Fix cavets
    //  - CS is not reliable it won't work if user code is inside a syscall.
    //  - two syscall won't be able to execute at a time.
    // Potential fix: ensure the process_scheduler won't execute during a syscall.
    int pid = get_idt_reverse_pid_lookup(e_cs);
    int npid = process_scheduler_get_next_pid(pid);

    if (pid != npid) {
        print_log("[process_scheduler] pid: %d -> %d", pid, npid);
        struct Process *process = get_process(pid);
        process->state = STATE_READY;
        process->cs = e_cs;
        process->ip = e_ip;
        process->ss = e_ss;
        process->sp = e_sp;

        struct Process *nprocess = get_process(npid);
        nprocess->state = STATE_RUNNING;
        e_cs = nprocess->cs;
        e_ip = nprocess->ip;
        e_ss = nprocess->ss;
        e_sp = nprocess->sp;
    }

    *_e_ip = e_ip;
    *_e_cs = e_cs;
    *_e_sp = e_sp;
    *_e_ss = e_ss;
}