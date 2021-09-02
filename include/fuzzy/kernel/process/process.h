#pragma once
#include <process.h>
#include<stddef.h>

#define MAX_PROCESS 10
#define GDT_TABLE_SIZE MAX_PROCESS*2+3

typedef char ARGV[PROCESS_MAX_ARGC][PROCESS_MAX_ARG_LEN];

enum process_state{
    STATE_COLD = 0,  // STATE_COLD must be 0
    STATE_LOADING,  // resources allocated but not ready.
    STATE_READY,
    STATE_RUNNING,
    STATE_EXIT,    // should be unallocated in next scheduling cycle
    STATE_BLOCK,    // process is waiting on IO, process_wait, etc.
};

enum process_state_block {
    STATE_BLOCK_PROCESS_WAIT,   // waiting for another process termination.
};

struct Process {
    enum process_state state;

    unsigned int ss, cs, sp, ip;
    unsigned int *e;

    int status_code;

    unsigned int ppid;  // parent pid, 0 to root under kernel_core

    enum process_state_block block_type;  // valid, if state == BLOCK
    union {
        struct {
            unsigned int blocked_on_pid;
            unsigned int blocking_pid;  // TODO: make list
        } process_wait;
    } block_data;    // valid if state == BLOCK

    // schedule for IRQ0
    // 0  - no fork requested or last fork successful; can try fork again
    // 1  - fork requested; should NOT try fork for now.
    // -1 - last fork failed; can try fork again
    signed char flagirq0_fork_ready;
    int flagirq0_fork_newchild;  // pid
};

void process_scheduler_init();

int syscall_1_process(int operation, int a0, int a1, int a2, int a3, int user_ds);

// getter
struct Process *get_process(int pid);
int get_gdt_number_from_entry_id(int id);

// allocation
int get_idt_cs_entry(int process_id);
int get_idt_ds_entry(int process_id);
int get_idt_reverse_pid_lookup_cs(int cs);
int get_idt_reverse_pid_lookup_ds(int ds);

// process create or kill
int process_create(unsigned int ppid, int argc, char *argv[]);
void process_kill(unsigned int pid, int status);

// scheduler

// on return process_scheduler, irq0 should execute from
// cs:eip with ss:esp (updated or not).
void process_scheduler(int *_e_ip, int *_e_cs, int *_e_sp, int *_e_ss);

// user space <-> kernel space data transfer helper
extern void syscall_strncpy_user_to_kernel(int user_ds, char *src_es_address, char *dest_ds_address, size_t size);
extern void syscall_strncpy_kernel_to_user(int user_ds, char *dest_address, char *src_address, size_t size);
extern void kernel_memncpy_absolute(int dst_ds, char *dst_address, int src_ds, char *src_address, size_t size);

// operations
int process_waitpid(unsigned int pid, unsigned int blocked_on_pid);

int process_fork_mark_ready(int pid);
int process_fork_check_ready(int pid);
