#include <process.h>
#include <stdarg.h>
#include <stddef.h>
#include <sys/syscall.h>

int spawnl(const char *file_path, const char *arg0, ...) {
    va_list args;
    va_start(args, arg0);
    const char *argv[PROCESS_MAX_ARGC] = {NULL};

    // prepare argv
    {
        argv[0] = arg0;
        int i = 1;
        while (i < PROCESS_MAX_ARGC - 2) {
            char *arg = va_arg(args, char *);
            if (arg == NULL)
                break;
            argv[i] = arg;
            i++;
        }
    }

    int rv = _spawnv_syscall(file_path, argv);
    va_end(args);
    return rv;
}

int spawnv(const char *file_path, const char *argv[]) {
    // kernel expects argv size must be PROCESS_MAX_ARGC
    const char *argv_resized[PROCESS_MAX_ARGC] = {NULL};
    for (int i = 0; i < PROCESS_MAX_ARGC - 1; i++) {
        if (argv[i] == NULL)
            break;
        argv_resized[i] = argv[i];
    }

    return _spawnv_syscall(file_path, argv_resized);
}

int _spawnv_syscall(const char *file_path, const char *argv[]) {
    // kernel expects argv size must be PROCESS_MAX_ARGC
    int pid = SYSCALL_A3(SYSCALL_PROCESS, SYSCALL_PROCESS_SUB_SPAWN_FNAME,
                         file_path, argv);
    return pid;
}

void yield() { __asm__("int $0x20"); }

int waitpid(unsigned int blocked_on_pid, int *exit_code) {
    while (1) {
        int status = SYSCALL_A3(SYSCALL_PROCESS, SYSCALL_PROCESS_SUB_WAIT,
                                blocked_on_pid, exit_code);
        if (status < 0) {
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

int getpid() {
    return SYSCALL_A2(SYSCALL_PROCESS, SYSCALL_PROCESS_SUB_GET,
                      SYSCALL_PROCESS_SUB_GET_PID);
}

int fork() {
    int mark_it = SYSCALL_A2(SYSCALL_PROCESS, SYSCALL_PROCESS_SUB_FORK,
                             SYSCALL_PROCESS_SUB_FORK_MARK_READY);
    if (mark_it != 0) {
        // failed to mark process as fork ready, maybe it's already marked.
        return mark_it;
    }
    // fork requested.
    // let's process scheduler fork the job.
    yield();

    // fork request should be complete by now.
    int status = SYSCALL_A2(SYSCALL_PROCESS, SYSCALL_PROCESS_SUB_FORK,
                            SYSCALL_PROCESS_SUB_FORK_CHECK_READY);
    if (status < 0) {
        // request either failed
        // or still waiting: it's a bad state, but we are going to let that
        // slide.
        return status;
    }
    // fork successful.
    int child_pid = status;
    int my_pid = getpid();
    if (my_pid == child_pid) {
        // child process
        return 0;
    }
    // parent process
    return child_pid;
}