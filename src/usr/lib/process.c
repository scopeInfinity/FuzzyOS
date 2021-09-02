#include <process.h>
#include <stddef.h>
#include <stdarg.h>
#include <sys/syscall.h>

int spawnl(char *file_path, char *arg0, ...) {
    va_list args;
    va_start(args, arg0);
    char *argv[PROCESS_MAX_ARGC] = {NULL};

    // prepare argv
    {
        argv[0] = arg0;
        int i = 1;
        while(i<PROCESS_MAX_ARGC-2) {
            char *arg = va_arg(args, char *);
            if(arg==NULL) break;
            argv[i] = arg;
            i++;
        }
    }

    int rv = _spawnv_syscall(file_path, argv);
    va_end(args);
    return rv;
}

int spawnv(char *file_path, char *argv[]) {
    // kernel expects argv size must be PROCESS_MAX_ARGC
    char *argv_resized[PROCESS_MAX_ARGC] = {NULL};
    for (int i = 0; i < PROCESS_MAX_ARGC - 1; i++) {
        if (argv[i] == NULL) break;
        argv_resized[i] = argv[i];
    }

    return _spawnv_syscall(file_path, argv_resized);
}


int _spawnv_syscall(char *file_path, char *argv[]) {
    // kernel expects argv size must be PROCESS_MAX_ARGC
    int pid = SYSCALL_A3(SYSCALL_PROCESS, SYSCALL_PROCESS_SUB_SPAWN_FNAME, file_path, argv);
    return pid;
}

void yield() {
    __asm__("int $0x20");
}

int waitpid(unsigned int blocked_on_pid) {
    while(1) {
        int status = SYSCALL_A2(SYSCALL_PROCESS, SYSCALL_PROCESS_SUB_WAIT, blocked_on_pid);
        if(status < 0) {
            // err
            return status;
        }
        if (status == 0) {
            // wait over
            return 0;
        }
        // keep waiting
        yield();
    }
}