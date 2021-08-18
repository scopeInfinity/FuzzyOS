#pragma once

#define SYSCALL_PROCESS_SUB_EXIT         0
#define SYSCALL_PROCESS_SUB_SPAWN_LBA_SC 1
#define SYSCALL_PROCESS_SUB_EXEC_LBA_SC  2
#define SYSCALL_PROCESS_SUB_SPAWN_FNAME  3

#define PROCESS_MAX_ARGC     6
#define PROCESS_MAX_ARG_LEN  32

// if non-negative value is returned
//   then it's the pid of the new process
//   otherwise it's some error code
int spawnl(char *path, char *arg0, ...);
int spawnv(char *path, char *argv[]);

void yield();
