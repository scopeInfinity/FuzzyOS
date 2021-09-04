#include <fuzzy/kernel/process/process.h>
#include <fuzzy/kernel/panic.h>

int process_scheduler_rr(int lastpid) {
    // Round Robin Scheduler.
    int best_id = -1;  // not found
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
    int best_id = -1;  // not found
    for (int nid = MAX_PROCESS-1; nid >= 0; --nid) {
        struct Process *process = get_process(nid);

        if (process->state == STATE_READY) {
            best_id = nid;
            break;
        }
    }
    return best_id;
}

void process_scheduler_stash_state() {
    for (int id = 0; id < MAX_PROCESS; id++) {
        struct Process *process = get_process(id);

        if(process->state == STATE_EXIT) {
            // process unallocate ready to be killed
            process->state = STATE_COLD;
            // print_log("process_killed: %d", id);
        } else if(process->state == STATE_RUNNING) {
            // process move running process to ready
            process->state = STATE_READY;
        }
    }
}

int process_scheduler_get_next_pid(int lastpid) {
    // execute one of the scheduling algorithm
    return process_scheduler_rr(lastpid);
}

static void handle_fork(unsigned int ppid, struct Process *process) {
    if(process->flagirq0_fork_ready>0) {
        int npid = process_fork(ppid);
        if(npid<0) {
            process->flagirq0_fork_ready = -1; // request failed;
        } else {
            struct Process *nprocess = get_process(npid);
            process->flagirq0_fork_ready = 0;  // request success;
            process->flagirq0_fork_newchild = npid;
            nprocess->flagirq0_fork_ready = 0; // request success;
            nprocess->flagirq0_fork_newchild = npid;
        }
    }
}

void process_scheduler(int *_e_ip, int *_e_cs, int *_e_sp, int *_e_ss) {
    int e_ip = *_e_ip;
    int e_cs = *_e_cs;
    int e_sp = *_e_sp;
    int e_ss = *_e_ss;
    print_info("[process_scheduler] cs:ip %x:%x,  ss:sp %x:%x",
        e_cs, e_ip,
        e_ss, e_sp);

    int pid = get_idt_reverse_pid_lookup_cs(e_cs);

    struct Process *process = get_process(pid);
    process_scheduler_stash_state();

    if(process->state != STATE_COLD) {
        // if last process is still alive
        process->cs = e_cs;
        process->ip = e_ip;
        process->ss = e_ss;
        process->sp = e_sp;
        handle_fork(pid, process);
    }

    int npid = process_scheduler_get_next_pid(pid);

    if(npid<0) {
        PANIC(0, "[process_scheduler] no STATE_READY process alive");
    }

    if(pid != npid) {
        print_info("[process_scheduler] pid: %d -> %d", pid, npid);
    }

    struct Process *nprocess = get_process(npid);
    nprocess->state = STATE_RUNNING;
    e_cs = nprocess->cs;
    e_ip = nprocess->ip;
    e_ss = nprocess->ss;
    e_sp = nprocess->sp;

    *_e_ip = e_ip;
    *_e_cs = e_cs;
    *_e_sp = e_sp;
    *_e_ss = e_ss;
}

int process_waitpid(unsigned int pid, unsigned int blocked_on_pid, int *exit_code) {
    // It currently allows blocking on any pid, and rely on syscall client
    // for yield.
    struct Process *other_process = get_process(blocked_on_pid);
    if(other_process==NULL) {
        return -1;  // err
    }
    if(other_process->state == STATE_COLD) {
        *exit_code = other_process->exit_code;
        return 0;  // no error, wait over
    }
    return 1; // no error, keep waiting
}
