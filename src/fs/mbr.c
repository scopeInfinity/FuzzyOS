#include <fuzzy/fs/ffs.h>
#include <fuzzy/fs/mbr.h>
#include <drivers/disk/disk.h>
#include <lib/utils/logging.h>
#include <lib/utils/panic.h>

char _cache_mbrblock[FS_BLOCK_SIZE];
int _cache_mbrblock_ready = 0;

char *get_mbrblock() {
    if(!_cache_mbrblock_ready) {
        int memory_location = ((int)_cache_mbrblock)+MEMORY_LOCATION_KERNEL;
        int lba = 0;
        int err = load_sectors(memory_location, 0x80, lba, 1);
        if (err) {
            PANIC(err, "[mbr] Failed to read MBR block.");
        }
        _cache_mbrblock_ready = 1;
    }
    return _cache_mbrblock;
}


void read_partition_entry(int id, struct PartitionEntry *entry) {
    // TODO: Handle all partition id.
    char *mbr_block = get_mbrblock();
    char *entry_str=(char*)entry;

    memcpy(entry_str, mbr_block+MBR_PARTITION_BEGIN, sizeof(struct PartitionEntry));
    print_log("[mbr][p%d] lba: %d, sector_count: %d",
        id, entry->lba, entry->sector_count);
}

