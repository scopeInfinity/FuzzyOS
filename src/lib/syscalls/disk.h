#ifndef __LIB_SYSCALLS_DISK
#define __LIB_SYSCALLS_DISK

extern int _low_disk_read_sectors(
    unsigned char count,
    unsigned short cylinder_sector,  // 10 bit + 6bit
    unsigned short head_driveindex, // 8 bit + 8bit
    unsigned short memory_address);

int load_sectors(unsigned short address,
                 unsigned char drive,
                 unsigned int sector_index, // 1-based
                 unsigned char count) {
    _low_disk_read_sectors(
        count,
        ((sector_index>>8)&0xFFC0) | (sector_index&0x3F),
        ((sector_index>>16)&0xF00) | (drive),
        address);
}

#endif