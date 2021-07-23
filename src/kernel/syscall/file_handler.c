#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "fs/ffs.h"
#include "kernel/extern.h"
#include <lib/utils/logging.h>

int fh_switch(int operation) {
    return 0;
}

int _file_handler_open(char *filename) {
    char local_filename[FS_FFS_FILENAME_LIMIT];
    syscall_strccpy_es_to_ds(filename, local_filename, sizeof(local_filename));
    syscall_context_switching_fix_es();
    return strlen(local_filename);
}

int syscall_2_file_handler(int operation, int a1, int a2, int a3) {
    switch (operation) {
        case SYSCALL_FILE_SUB_OPEN:
            return _file_handler_open((char*)a1);
            break;
    }
    return -1;
}

