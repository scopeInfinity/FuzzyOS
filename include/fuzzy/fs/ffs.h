#pragma once
/*
Format
metadata;
fileentry0; 128 byte each
fileentry1
fileentry2
fileentry3
...
fileentry127; 16896 bytes or 33 block used.
data...
*/

/*
Number of file which can be stored: 64
*/

#include <stddef.h>
#include <stdint.h>

#define FS_FFS_FILENAME_LIMIT 100 // including NULL, same as dirent.h
#define FFS_UNIQUE_PARITION_ID 0  // only paritition 0 is supported for now

enum FFSFileFlagMask { FFS_FILE_FLAG_EXECUTABLE = 1 << 0 };

union FFSMetaData {
    struct {
        char signature[16];
    } content;
    char bytes[512];
}; // size: 512 bytes

union FFSFileEntry {
    struct {
        int32_t start_block_id; // 0 implies no file.
        int32_t filesize;
        char filename[FS_FFS_FILENAME_LIMIT];
        uint32_t flags;
    } content;
    char bytes[128];
}; // size: 128 bytes

#define FS_FFS_FIRST_BLOCK_SIZE sizeof(union FFSMetaData)
#define FS_FFS_FILEENTRY_SIZE sizeof(union FFSFileEntry)
#define FS_FFS_FILEENTRY_COUNT 128

#define FS_BLOCK_SIZE 512
#define FS_FFS_BLOCK_DATA_START                                                \
    ((FS_FFS_FIRST_BLOCK_SIZE +                                                \
      FS_FFS_FILEENTRY_SIZE * FS_FFS_FILEENTRY_COUNT) /                        \
     FS_BLOCK_SIZE)

#define FS_FFS_SIGNATURE "__FuzzyOS__FFS__" // 16 chars

int resolve_abs_lba(int parition_id, int partition_relative_lba);

int partition_read_block(int block_index, void *wr_buffer);

int verify_partition(int partition_id);

int fetch_file_entry(int partition_id, int entry_id, union FFSFileEntry *entry);

int fetch_file_content(const int partition_id, const union FFSFileEntry *entry,
                       char buffer[FS_BLOCK_SIZE], const int file_block_id);