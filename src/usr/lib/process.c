#include <process.h>
#include <stddef.h>
#include <stdarg.h>
#include <sys/syscall.h>

int spawnl(char *file_path, char *arg0, ...) {
    va_list args;
    va_start(args, arg0);
    char *argv[PROCESS_MAX_ARGC];

    // prepare argv
    {
        int i = 0;
        while(i<PROCESS_MAX_ARGC-1) {
            char *arg = va_arg(args, char *);
            if(arg==NULL) break;
            argv[i] = arg;
            i++;
        }
        argv[i] = NULL;
    }

    int rv = spawnv(file_path, argv);
    va_end(args);
    return rv;
}

int spawnv(char *file_path, char *argv[]) {
    int pid = SYSCALL_A3(SYSCALL_PROCESS, SYSCALL_PROCESS_SUB_SPAWN_FNAME, file_path, argv);
    if(pid>=0) yield();
    return pid;
}

void yield() {
    __asm__("int $0x20");
}