#pragma once

#define MAX_PROCESS 10
// GDT_TABLE_SIZE = MAX_PROCESS*2+3
#define GDT_TABLE_SIZE 23

enum process_state{
    STATE_COLD = 0,  // STATE_COLD must be 0
    STATE_LOADING,  // resources allocated but not ready.
    STATE_READY,
    STATE_RUNNING,
    STATE_EXIT,
};

struct Process {
    enum process_state state;

    unsigned short ss, cs, ds;
    unsigned int *e;

    int exit_code;
};

void process_scheduler_init();

int syscall_1_process(int operation, int a0, int a1, int a2, int a3, int user_ds);

// getter
struct Process *process_get(int pid);

// allocation
int get_idt_cs_entry(int process_id);
int get_idt_ds_entry(int process_id);
int get_idt_reverse_pid_lookup(int cs);

// scheduler

// on return process_scheduler, irq0 should execute from
// cs:eip with ss:esp (updated or not).
void process_scheduler(int *_e_ip, int *_e_cs, int *_e_sp, int *_e_ss);
