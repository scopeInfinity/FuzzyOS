#include <lib/utils/process.h>

int exec(int sector_start, int sector_count) {
    return syscall_process(sector_start, sector_count);
}
