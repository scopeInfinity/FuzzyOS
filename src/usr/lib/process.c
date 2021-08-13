#include <process.h>
#include <sys/syscall.h>

int spawn(char *path) {
    return SYSCALL_A3(SYSCALL_PROCESS, SYSCALL_PROCESS_SUB_SPAWN_LBA_SC, SECTOR_START_APP_LS, SECTOR_COUNT_APP_LS);
}
