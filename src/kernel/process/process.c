#include <fuzzy/fs/ffs.h>
#include <fuzzy/kernel/interrupts/timer.h>
#include <fuzzy/kernel/process/process.h>
#include <fuzzy/memmgr/stackguard/stackguard.h>

#include <process.h>

#include <lib/utils/logging.h>
#include <lib/utils/output.h>

int process_spawn(int lba_index, int sector_count, unsigned int ppid, int argc,
                  char *argv[]) {
    print_info("[process_spawn] create");
    int pid = process_create(ppid, argc, argv);
    if (pid < 0) {
        print_log("Failed to reserved a new pid");
        return -1;
    }
    print_info("[process_spawn] loading, pid: %d", pid);

    int err = process_load_from_disk(pid, lba_index, sector_count);
    if (err) {
        print_log("Failed to load app in memory, Error: ", err);
        return -2;
    }
    print_info("[process_spawn] ready, pid: %d", pid);
    struct Process *process = get_process(pid);
    process->state = STATE_READY;
    VERIFY_STACKGUARD();
    return pid;
}

int process_exec(int lba_index, int sector_count) {
    // Not yet implemented
    return -1;
}

int syscall_1_process_exit(int pid, int exit_code) {
    process_kill(pid, exit_code);
    return 0;
}

int syscall_1_process_wait(int user_ds, int pid, int blocked_on_pid,
                           int *_us_exit_code) {
    int exit_code;
    int return_value = process_waitpid(pid, blocked_on_pid, &exit_code);
    if (_us_exit_code != NULL) {
        // forward exit_code only if user wants it
        syscall_strncpy_kernel_to_user(user_ds, _us_exit_code, &exit_code,
                                       sizeof(exit_code));
    }
    return return_value;
}

int syscall_1_process_fork(int user_pid, int op) {
    switch (op) {
    case SYSCALL_PROCESS_SUB_FORK_MARK_READY:
        return process_fork_mark_ready(user_pid);
    case SYSCALL_PROCESS_SUB_FORK_CHECK_READY:
        return process_fork_check_ready(user_pid);
    }
    return -1;
}

int syscall_1_process_get(int pid, int op) {
    switch (op) {
    case SYSCALL_PROCESS_SUB_GET_PID:
        return pid;
    }
    return -2;
}

int syscall_1_process_spawn_fname(unsigned int user_pid, int user_ds,
                                  char *_us_filename, char *_us_argv[]) {
    // User must send all PROCESS_MAX_ARGC arguments.
    char *argv_with_uspointer[PROCESS_MAX_ARGC];
    char filename[FS_FFS_FILENAME_LIMIT];
    int argc = 0;    // kernel mode
    ARGV argv = {0}; // kernel mode
    syscall_strncpy_user_to_kernel(user_ds, _us_filename, filename,
                                   sizeof(filename));
    syscall_strncpy_user_to_kernel(user_ds, _us_argv, argv_with_uspointer,
                                   sizeof(argv_with_uspointer));
    // if src string is NULL, then dst string also should be null.
    for (int i = 0; i < PROCESS_MAX_ARGC - 1; i++) {
        if (argv_with_uspointer[i] == NULL) {
            break;
        }
        syscall_strncpy_user_to_kernel(user_ds, argv_with_uspointer[i], argv[i],
                                       sizeof(argv[i]));
        argc++;
    }

    union FFSFileEntry entry;
    int file_id = file_handler_find(filename, &entry);
    if (file_id < 0)
        return file_id;
    if (!(entry.content.flags & FFS_FILE_FLAG_EXECUTABLE)) {
        // allow only executable files to spawn
        return -1;
    }

    int lba_start =
        resolve_abs_lba(FFS_UNIQUE_PARITION_ID, entry.content.start_block_id);
    int sector_count =
        (entry.content.filesize + FS_BLOCK_SIZE - 1) / FS_BLOCK_SIZE;

    return process_spawn(lba_start, sector_count, user_pid, argc, argv);
}

int syscall_1_process(int operation, int a0, int a1, int a2, int a3,
                      int user_ds) {
    int user_pid = get_idt_reverse_pid_lookup_ds(user_ds);
    switch (operation) {
    case SYSCALL_PROCESS_SUB_EXIT:
        syscall_1_process_exit(user_pid, a0);
        return 0;
    case SYSCALL_PROCESS_SUB_WAIT:
        return syscall_1_process_wait(user_ds, user_pid, a0, a1);
    case SYSCALL_PROCESS_SUB_FORK:
        return syscall_1_process_fork(user_pid, a0);
    case SYSCALL_PROCESS_SUB_GET:
        return syscall_1_process_get(user_pid, a0);
    case SYSCALL_PROCESS_SUB_SPAWN_FNAME:
        return syscall_1_process_spawn_fname(user_pid, user_ds, (char *)a0,
                                             (char **)a1);
    }
    return -1;
}
