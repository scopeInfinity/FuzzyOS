#include <fuzzy/fs/ffs.h>

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>

#include <lib/utils/logging.h>

int fh_switch(int operation) {
    return 0;
}

int file_handler_find(char *filename, union FFSFileEntry *entry) {
    // search for filename case insensitive as keyboard driver currently
    // doesn't support shift or caps lock.
    int file_id = 0;
    while (file_id < FS_FFS_FILEENTRY_COUNT) {
        int err = fetch_file_entry(
            FFS_UNIQUE_PARITION_ID, file_id, entry);
        if(!err && strcmpi(filename, entry->content.filename)==0) {
            // match
            return file_id;
        }
        file_id++;
    }
    return -1; // no file found
}

int _file_handler_open(int user_ds, char *_us_filename) {
    char filename[FS_FFS_FILENAME_LIMIT];
    syscall_strncpy_user_to_kernel(user_ds, _us_filename, filename, sizeof(filename));

    union FFSFileEntry entry;
    return file_handler_find(filename, &entry);
}

int _file_handler_read(int user_ds, int file_id, char _us_buffer[FILEIO_BUFFER_SIZE], size_t byte_index) {
    union FFSFileEntry entry;
    int err = fetch_file_entry(
        FFS_UNIQUE_PARITION_ID, file_id, &entry);
    if(err) {
        return -2;
    }

    int block_id = byte_index/FS_BLOCK_SIZE;
    int byte_index_block = byte_index%FS_BLOCK_SIZE;
    char block[FS_BLOCK_SIZE];

    int len = fetch_file_content(
        FFS_UNIQUE_PARITION_ID,
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

int _file_handler_read_dir(int user_ds, int last_file_id, struct dirent *_us_dirent) {
    // char *_us_filename
    union FFSFileEntry entry;
    int file_id;
    if (last_file_id < 0) {
        file_id = 0;
    } else {
        file_id = last_file_id+1;
    }
    while (file_id < FS_FFS_FILEENTRY_COUNT) {
        int err = fetch_file_entry(
            FFS_UNIQUE_PARITION_ID, file_id, &entry);
        if(err) {
            file_id++;
            continue;
        }
        // next file
        struct dirent centry = {0};
        memcpy(centry.d_name, entry.content.filename, sizeof(centry.d_name));
        centry.size = entry.content.filesize;
        if(entry.content.flags&FFS_FILE_FLAG_EXECUTABLE) {
            centry.flag |= DIRENT_EXECUTABLE;
        }

        syscall_strncpy_kernel_to_user(
            user_ds, _us_dirent,
            &centry, sizeof(struct dirent));
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
            return _file_handler_read_dir(user_ds, a1, (struct dirent*)a2);
    }
    return -1;
}

