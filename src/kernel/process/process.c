#include <fuzzy/fs/ffs.h>
#include <fuzzy/kernel/process/process.h>
#include <fuzzy/kernel/interrupts/timer.h>

#include <process.h>

#include <lib/utils/logging.h>
#include <lib/utils/output.h>

extern int call_main(int cs, int ds, int argc, char *argv[]);

int process_spawn(int lba_index, int sector_count) {
    print_info("[process_spawn] create");
    int pid = process_create();
    if(pid<0) {
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
    return 0;
}

int process_exec(int lba_index, int sector_count) {
    // Not yet implemented
    return -1;
}

int syscall_1_process_exit(int user_ds, int status) {
    process_kill(user_ds, status);
    return 0;
}

int syscall_1_process_spawn_lba_sc(int lba_start, int sector_count) {
    return process_spawn(lba_start, sector_count);
}

int syscall_1_process_exec_lba_sc(int lba_start, int sector_count) {
    return process_exec(lba_start, sector_count);
}

int syscall_1_process_spawn_fname(int user_ds, char *_us_filename, char *_us_argv[]) {
    char filename[FS_FFS_FILENAME_LIMIT];
    char argv_with_uspointer[PROCESS_MAX_ARGC];
    char *argv_with_kspointer[PROCESS_MAX_ARGC];
    char argv[PROCESS_MAX_ARGC][PROCESS_MAX_ARG_LEN];  // data store
    syscall_strncpy_user_to_kernel(user_ds, _us_filename, filename, sizeof(filename));
    syscall_strncpy_user_to_kernel(user_ds, _us_argv, argv_with_uspointer, sizeof(argv_with_uspointer));
    // if src string is NULL, then dst should be null.
    for (int i = 0; i < PROCESS_MAX_ARGC; i++) {
        if(argv_with_uspointer[i]==NULL) {
            argv_with_kspointer[i]=NULL;
            break;
        }
        syscall_strncpy_user_to_kernel(user_ds, argv[i], argv_with_uspointer[i], sizeof(argv[i]));
        argv_with_kspointer[i] = argv[i];
    }

    union FFSFileEntry entry;
    int file_id = file_handler_find(filename, &entry);
    if (file_id < 0) return file_id;


    int lba_start = resolve_abs_lba(FFS_UNIQUE_PARITION_ID, entry.content.start_block_id);
    int sector_count = (entry.content.filesize + FS_BLOCK_SIZE -1)/FS_BLOCK_SIZE;

    // TODO: process created should have argv pushed on top of stack.
    return syscall_1_process_spawn_lba_sc(lba_start, sector_count);
}

int syscall_1_process(int operation, int a0, int a1, int a2, int a3, int user_ds) {
    switch (operation) {
        case SYSCALL_PROCESS_SUB_EXIT:
            syscall_1_process_exit(user_ds, a0);
            return 0;
        case SYSCALL_PROCESS_SUB_SPAWN_LBA_SC:
            return syscall_1_process_spawn_lba_sc(a0, a1);
        case SYSCALL_PROCESS_SUB_EXEC_LBA_SC:
            return syscall_1_process_exec_lba_sc(a0, a1);
        case SYSCALL_PROCESS_SUB_SPAWN_FNAME:
            return syscall_1_process_spawn_fname(user_ds, (char*)a0, (char**)a1);
    }
    return -1;
}
