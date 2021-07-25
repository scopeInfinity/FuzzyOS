#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <fuzzy/fs/ffs.h>
#include "kernel/extern.h"
#include <lib/utils/logging.h>

const int partition_id = 0;

int fh_switch(int operation) {
    return 0;
}

int _file_handler_open(int user_ds, char *_us_filename) {
    char filename[FS_FFS_FILENAME_LIMIT];
    syscall_strncpy_user_to_kernel(user_ds, _us_filename, filename, sizeof(filename));

    union FFSFileEntry entry;
    int file_id = 0;
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

int _file_handler_read(int user_ds, int file_id, char _us_buffer[FILEIO_BUFFER_SIZE], size_t byte_index) {
    union FFSFileEntry entry;
    int err = fetch_file_entry(
        partition_id, file_id, &entry);
    if(err) {
        return -2;
    }

    int block_id = byte_index/FS_BLOCK_SIZE;
    int byte_index_block = byte_index%FS_BLOCK_SIZE;
    char block[FS_BLOCK_SIZE];

    int len = fetch_file_content(
        partition_id,
        &entry,
        block,
        block_id);

    if (len < 0) {
        return len; // err
    }
    if (len == 0) {
        return 0;
    }
    len = min(len, FILEIO_BUFFER_SIZE); // guard for _us_buffer

    syscall_strncpy_kernel_to_user(user_ds, _us_buffer, block+byte_index_block, len);
    return len;
}

int _file_handler_read_dir(int user_ds, int last_file_id, char *_us_filename) {
    union FFSFileEntry entry;
    int file_id;
    if (last_file_id < 0) {
        file_id = 0;
    } else {
        file_id = last_file_id+1;
    }
    while (file_id < FS_FFS_FILEENTRY_COUNT) {
        int err = fetch_file_entry(
            partition_id, file_id, &entry);
        if(err) continue;
        // next file
        syscall_strncpy_kernel_to_user(
            user_ds, _us_filename,
            entry.content.filename, sizeof(entry.content.filename));
        return file_id;
    }
    return -1; // there is no next file
}

int syscall_2_file_handler(int operation, int a1, int a2, int a3, int user_ds) {
    switch (operation) {
        case SYSCALL_FILE_SUB_OPEN:
            return _file_handler_open(user_ds, (char*)a1);
        case SYSCALL_FILE_SUB_READBUFFER:
            return _file_handler_read(user_ds, a1, (char*)a2, a3);
        case SYSCALL_FILE_SUB_READ_DIR:
            return _file_handler_read_dir(user_ds, a1, (char*)a2);
    }
    return -1;
}

