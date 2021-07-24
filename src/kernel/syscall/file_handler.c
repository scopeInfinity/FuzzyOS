#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <fuzzy/fs/ffs.h>
#include "kernel/extern.h"
#include <lib/utils/logging.h>

int fh_switch(int operation) {
    return 0;
}

int _file_handler_open(char *_us_filename) {
    char filename[FS_FFS_FILENAME_LIMIT];
    syscall_strccpy_es_to_ds(_us_filename, filename, sizeof(filename));
    syscall_context_switching_fix_es();

    union FFSFileEntry entry;
    int file_id = 0;
    const int partition_id = 0;
    while (file_id < FS_FFS_FILEENTRY_COUNT) {
        int err = fetch_file_entry(
            partition_id, file_id, &entry);
        if(!err && strcmp(filename, entry.content.filename)==0) {
            // match
            return file_id;
        }
        file_id++;
    }
    return -1;
}

int _file_handler_read( int file_id, char* write_buffer, int start_byte, int size) {
    return 0;
}

int syscall_2_file_handler(int operation, int a1, int a2, int a3) {
    switch (operation) {
        case SYSCALL_FILE_SUB_OPEN:
            return _file_handler_open((char*)a1);
            break;
    }
    return -1;
}

