#include <lib/syscall/syscall.h>
#include <lib/utils/logging.h>
#include <lib/utils/process.h>

int exec(int sector_start, int sector_count) {
    int id = syscall_process(sector_start, sector_count);
    if(id<0) {
        print_log("syscall_process(%d, %d) failed to create process.",
            sector_start, sector_count);
        return -1;
    }

    // Following is not a good solution but works for now.
    while(1) {
        // TODO: Use enum instead of 0.
        if(syscall_process_state(id)==0) {
            return syscall_process_exitcode(id);
        }
        process_yield();
    }
}

int fork() {
    return syscall_process_fork();
}

void process_yield() {
    // TODO: Implement.
    for (int i = 0; i < 1000; ++i);
}