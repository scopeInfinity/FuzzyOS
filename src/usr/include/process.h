#pragma once

#define SYSCALL_PROCESS_SUB_EXIT         0
#define SYSCALL_PROCESS_SUB_WAIT         1
#define SYSCALL_PROCESS_SUB_FORK         2
#define SYSCALL_PROCESS_SUB_SPAWN_FNAME  3
#define SYSCALL_PROCESS_SUB_GET          4

#define PROCESS_MAX_ARGC     6
#define PROCESS_MAX_ARG_LEN  32

#define SYSCALL_PROCESS_SUB_FORK_MARK_READY   0
#define SYSCALL_PROCESS_SUB_FORK_CHECK_READY  1

#define SYSCALL_PROCESS_SUB_GET_PID 0

// if non-negative value is returned
//   then it's the pid of the new process
//   otherwise it's some error code
int spawnl(char *path, char *arg0, ...);
int spawnv(char *path, char *argv[]);

int getpid();

int fork();
void yield();
int waitpid(unsigned int blocked_on_pid);
