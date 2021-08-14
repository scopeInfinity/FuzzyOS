#pragma once
#include<stddef.h>

#define MAX_PROCESS 10
// GDT_TABLE_SIZE = MAX_PROCESS*2+3
#define GDT_TABLE_SIZE 23

enum process_state{
    STATE_COLD = 0,  // STATE_COLD must be 0
    STATE_LOADING,  // resources allocated but not ready.
    STATE_READY,
    STATE_RUNNING,
    STATE_EXIT,    // should be unallocated in next scheduling cycle
};

struct Process {
    enum process_state state;

    unsigned int ss, cs, sp, ip;
    unsigned int *e;

    int status_code;
};

void process_scheduler_init();

int syscall_1_process(int operation, int a0, int a1, int a2, int a3, int user_ds);

// getter
struct Process *get_process(int pid);
int get_gdt_number_from_entry_id(int id);

// allocation
int get_idt_cs_entry(int process_id);
int get_idt_ds_entry(int process_id);
int get_idt_reverse_pid_lookup(int cs);

// process create or kill
int process_create();
void process_kill(int user_ds, int status);

// scheduler

// on return process_scheduler, irq0 should execute from
// cs:eip with ss:esp (updated or not).
void process_scheduler(int *_e_ip, int *_e_cs, int *_e_sp, int *_e_ss);


// user space <-> kernel space data transfer helper
extern void syscall_strncpy_user_to_kernel(int user_ds, char *src_es_address, char *dest_ds_address, size_t size);
extern void syscall_strncpy_kernel_to_user(int user_ds, char *dest_address, char *src_address, size_t size);
