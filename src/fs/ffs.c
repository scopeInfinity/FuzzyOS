#include <fuzzy/fs/ffs.h>
#include <fuzzy/fs/mbr.h>
#include <fuzzy/memmgr/layout.h>
#include <string.h>
#include <drivers/disk/disk.h>
#include <lib/utils/logging.h>

// TODO: cache partition entry
// TODO: verify partition metadata

int partition_read_block(int lba, void *wr_buffer) {
    int memory_location = ((int)wr_buffer)+MEMORY_KERNEL_LOCATION;
    int err = load_sectors(memory_location, 0x80, lba, 1);
    return err;
}

int verify_partition(int partition_id) {
    // cache
    // -1: not yet verified
    //  0: verified with no error
    //  1: verified but failed
    static char verified_cache[4] = {-1, -1, -1, -1};
    if(partition_id<0 || partition_id>=4) return 1;
    if(verified_cache[partition_id]!=-1) return verified_cache[partition_id];

    // fetch parition info
    struct PartitionEntry partition;
    read_partition_entry(partition_id, &partition);

    union FFSMetaData block;
    int err = partition_read_block(
        partition.lba, // FS metadata
        block.bytes);
    if (!err) {
        // verify signature
        err = strcmp(block.content.signature, FS_FFS_SIGNATURE);
    }
    if (err) {
        verified_cache[partition_id] = 1; // err
        print_log("[ffs] partition %d verification failed, err: %d", partition_id, err);
        return err;
    }
    verified_cache[partition_id] = 0; // no error
    return 0;
}

int resolve_abs_lba(int partition_id, int partition_relative_lba) {
    // fetch parition info
    struct PartitionEntry partition;
    read_partition_entry(partition_id, &partition);
    return partition.lba + partition_relative_lba;
}

int fetch_file_entry(
    int partition_id,
    int entry_id,
    union FFSFileEntry *entry) {
    int err = verify_partition(partition_id);
    if (err) {
        return err;
    }

    char buffer[FS_BLOCK_SIZE];
    // fetch parition info
    struct PartitionEntry partition;
    read_partition_entry(partition_id, &partition);

    // fetch file entry
    const int FILE_ENTRY_PER_BLOCK = (FS_BLOCK_SIZE/FS_FFS_FILEENTRY_SIZE);
    err = partition_read_block(
        partition.lba +
            1 + // FS metadata
            entry_id/FILE_ENTRY_PER_BLOCK,
        buffer);
    if (err) return err;

    memcpy(entry->bytes,
        buffer+(entry_id%FILE_ENTRY_PER_BLOCK)*FS_FFS_FILEENTRY_SIZE,
        sizeof(entry->bytes));
    int file_not_exists = (entry->content.start_block_id==0);
    err = file_not_exists;
    return err;
}

int fetch_file_content(
    const int partition_id,
    const union FFSFileEntry *entry,
    char buffer[FS_BLOCK_SIZE],
    const int file_block_id) {

    // fetch parition info
    struct PartitionEntry partition;
    read_partition_entry(partition_id, &partition);

    // fetch file entry
    int file_size = entry->content.filesize;
    int block_count = file_size/FS_BLOCK_SIZE;
    if(file_block_id >= block_count) {
        return -1;
    }

    int err = partition_read_block(
        partition.lba + entry->content.start_block_id + file_block_id,
        buffer);
    if (err) return -2;
    // returns content length in buffer.
    if(file_block_id + 1 == block_count) return file_size%FS_BLOCK_SIZE;
    return FS_BLOCK_SIZE;
}
