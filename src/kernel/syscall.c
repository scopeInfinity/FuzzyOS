#include <stddef.h>
#include <lib/utils/logging.h>
#include <drivers/keyboard/keyboard.h>

#include "kernel/process.c"
#include "kernel/file_handler.c"
#include "fs/ffs.h"

#define SYSCALL_SIZE 30

extern void syscall_copy_es_to_ss(char *es_address, char *ss_address, size_t size);

int SYSCALL_TABLE[SYSCALL_SIZE];

int syscall_0_keyboard_getch(int a0,int a1,int a2,int a3) {
    return keyboard_get_key_pressed_blocking();
}

int syscall_1_process_exec(int sector_start, int sector_count, int a2,int a3) {
    return process_exec(sector_start, sector_count);
}

// TODO: Think how to take string argument
// TODO: how to write on provide string buffer.
int syscall_2_file_handler(int operation, char *filename, int a2, int a3) {
    char local_filename[FS_FFS_FILENAME_LIMIT];
    syscall_copy_es_to_ss(filename, local_filename, sizeof(local_filename));
    return fh_switch(operation);
}

void register_syscalls() {
    print_log("Registering syscalls.");
    SYSCALL_TABLE[0]=syscall_0_keyboard_getch;
    SYSCALL_TABLE[1]=syscall_1_process_exec;
    SYSCALL_TABLE[2]=syscall_2_file_handler;
}