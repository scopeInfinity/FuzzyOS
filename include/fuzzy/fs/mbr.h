#pragma once

#define MBR_PARTITION_BEGIN 446

struct PartitionEntry {
    char drive;
    char start_chs[3];
    char partition_type;
    char end_chs[3];
    int lba;  // 4 bytes
    int sector_count;  // 4 bytes
};
