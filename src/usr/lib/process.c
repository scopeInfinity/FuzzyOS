#include <process.h>
#include <sys/syscall.h>

int spawn(char *file_path) {
    int pid = SYSCALL_A2(SYSCALL_PROCESS, SYSCALL_PROCESS_SUB_SPAWN_FNAME, file_path);
    if(pid>=0) yield();
    return pid;
}

void yield() {
    __asm__("int $0x20");
}