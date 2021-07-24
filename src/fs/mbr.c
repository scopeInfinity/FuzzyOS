#include <fuzzy/fs/ffs.h>
#include <fuzzy/fs/mbr.h>
#include <drivers/disk/disk.h>

void read_partition_entry(int id, struct PartitionEntry *entry) {
    char sector[FS_BLOCK_SIZE];
    char *entry_str=(char*)entry;
    partition_read_block(0, sector);

    memcpy(entry_str, sector+MBR_PARTITION_BEGIN, sizeof(struct PartitionEntry));
}

