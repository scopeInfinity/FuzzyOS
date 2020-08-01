#pragma once

#define DISK_KERNEL_SECTOR_START 77
#define DISK_KERNEL_SECTOR_COUNT 33

int load_sectors(unsigned int address,
                 unsigned char drive,
                 unsigned int sector_index, // 1-based
                 unsigned char count);