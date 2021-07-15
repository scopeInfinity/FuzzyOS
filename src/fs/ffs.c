#include <string.h>
#include <drivers/disk/disk.h>

int _partition_read_block(int block_index, void *wr_buffer) {
    // TODO: write block_index content to wr_buffer
}

void fetch_first_block(
    int (*partition_read_block)(void *dest),
    union FFSMetaData *block) {
    partition_read_block(0, block->bytes);
}

void fetch_file_entry(
    int (*partition_read_block)(void *dest),
    int entry_id,
    union FFSFileEntry *entry) {
    char buffer[FS_BLOCK_SIZE];
    const int FILE_ENTRY_PER_BLOCK = (FS_BLOCK_SIZE/FS_FFS_FILEENTRY_SIZE);
    partition_read_block(
        1+entry_id/FILE_ENTRY_PER_BLOCK,
        buffer);
    memcpy(entry->bytes,
        buffer+(entry_id%FILE_ENTRY_PER_BLOCK)*FS_BLOCK_SIZE,
        sizeof(entry->bytes));
}


