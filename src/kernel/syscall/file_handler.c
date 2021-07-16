#include <stddef.h>
#include <string.h>
#include "fs/ffs.h"
#include "kernel/extern.h"
#include <lib/utils/logging.h>

int fh_switch(int operation) {
    return 0;
}

// TODO: fix strncpy
// TODO: how to write on provide string buffer.
int syscall_2_file_handler(int operation, char *filename, int a2, int a3) {
    char local_filename[FS_FFS_FILENAME_LIMIT];
    syscall_strccpy_es_to_ds(filename, local_filename, sizeof(local_filename));
    syscall_context_switching_fix_es();
    return strlen(local_filename);
}

