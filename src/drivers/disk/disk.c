extern int _low_disk_read_sectors_real(
    unsigned char count,
    unsigned short cylinder_sector,  // 10 bit + 6bit
    unsigned short head_driveindex, // 8 bit + 8bit
    unsigned short memory_es,       // 16 bit
    unsigned short memory_address);   // 16bit

int load_sectors(unsigned int address,
                 unsigned char drive,
                 unsigned int sector_index, // 1-based
                 unsigned char count) {
    int memory_es = (address&0xF0000)>>4;
    address = address&0xFFFF;
    int cylinder_head = (sector_index/63);
    sector_index = sector_index%63 + 1;
    _low_disk_read_sectors_real(
        count,
        ((cylinder_head>>2)&0xFFC0) | (sector_index),
        ((cylinder_head<<8)&0xFF00) | (drive),
        memory_es,
        address);
}
