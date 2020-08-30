extern void process_handler_step();

// Max world_ticks goes to ~29 mins before overflowing.
#define INT_MAX_VALUE 0x7FFFFFFF

unsigned int interrupt_world_ticks_lo = 0;
unsigned int interrupt_world_ticks_hi = 0;

void interrupts_timer_add_ticks(unsigned int count) {
    unsigned int last_val_lo = interrupt_world_ticks_lo;
    interrupt_world_ticks_lo += count;
    if (last_val_lo<interrupt_world_ticks_lo) {
        interrupt_world_ticks_hi++;
    }
}

void interrupt_ticks_step(unsigned short ticks_jumped) {
    int t1 = interrupt_world_ticks_lo/PIC_PIT_FREQ;
    interrupts_timer_add_ticks(ticks_jumped);
    int t2 = interrupt_world_ticks_lo/PIC_PIT_FREQ;
    if(t1!=t2) {
        print_log(">> +1 second.");
    }
    process_handler_step();
}