#include <fuzzy/kernel/process/process.h>

int process_scheduler_get_next_pid(int lastpid) {
    // Round Robin Scheduler.
    int best_id = lastpid;
    for (int i = 1; i <= MAX_PROCESS; ++i) {
        int nid = (lastpid+i)%MAX_PROCESS;
        struct Process *process = process_get(nid);
        if (process->state == STATE_READY) {
            best_id = nid;
            break;
        }
    }
    return best_id;
}

void process_scheduler(int *_e_ip, int *_e_cs, int *_e_sp, int *_e_ss) {
    int e_ip = *_e_ip;
    int e_cs = *_e_cs;
    int e_sp = *_e_sp;
    int e_ss = *_e_ss;
    print_log("[process_scheduler] cs:ip %x:%x,  ss:sp %x:%x",
        e_cs, e_ip,
        e_ss, e_sp);
}