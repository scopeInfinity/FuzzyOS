#pragma once

// TODO: Fix diff 1 value in DISK_KERNEL_SECTOR_START
#define DISK_KERNEL_SECTOR_START 78
#define DISK_KERNEL_SECTOR_COUNT 25 //33

int load_sectors(unsigned short address,
                 unsigned char drive,
                 unsigned int sector_index, // 1-based
                 unsigned char count);