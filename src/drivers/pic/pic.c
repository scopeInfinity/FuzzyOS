#include <drivers/pic/pic.h>
#include <lib/utils/logging.h>

extern void pic_init_low();

extern void pic_timer_configure();
extern void pic_end_of_interrupt();

extern void pic_timer_set_counter(unsigned short counter);
extern unsigned short pic_timer_get_counter();
extern void pic_timer_reload_counter();

extern unsigned short pic_readmask();
extern void pic_writemask(unsigned short mask);

void pic_init() {
    print_log(LOG_PREFIX "PIC init.");
    pic_init_low();
    pic_timer_configure();

    // disable all IRQs
    pic_writemask(0xFFFF);
}

void pic_irq_enable(int irq) {
    int mask = pic_readmask();
    mask = mask&(~(1<<irq));
    pic_writemask(mask);
}

void pic_irq_disable(int irq) {
    int mask = pic_readmask();
    mask = mask|((1<<irq));
    pic_writemask(mask);
}