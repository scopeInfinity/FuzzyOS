#include <process.h>
#include <sys/syscall.h>

void spawn(char *path) {
    SYSCALL_A3(SYSCALL_PROCESS, SYSCALL_PROCESS_SUB_SPAWN_LBA_SC, 93, 19);
}
