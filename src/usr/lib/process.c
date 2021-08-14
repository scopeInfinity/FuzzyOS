#include <process.h>
#include <sys/syscall.h>

int spawn(char *file_path) {
    return SYSCALL_A2(SYSCALL_PROCESS, SYSCALL_PROCESS_SUB_SPAWN_FNAME, file_path);
}
