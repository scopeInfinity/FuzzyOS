// written for real mode

extern unsigned short _low_int_0x13(unsigned short ax, unsigned short bx,
                                    unsigned short cx, unsigned short dx,
                                    unsigned short es);

int load_sectors(unsigned int full_address, unsigned char drive,
                 unsigned int lba, // 0-based
                 unsigned int count) {
    // we don't expect disk_16 (used by bootloader) to load more than 255
    // sectors.

    int es = (full_address & 0xFFFF0) >> 4;
    int es_address = full_address & 0x000F;
    int cylinder_head = (lba / 63);
    int sector_index = lba % 63 + 1;
    // https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=02h:_Read_Sectors_From_Drive
    _low_int_0x13((0x02 << 8) | count, es_address,
                  ((cylinder_head >> 2) & 0xFFC0) | (sector_index),
                  ((cylinder_head << 8) & 0xFF00) | (drive), es);
    // https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=01h:_Get_Status_of_Last_Drive_Operation
    unsigned short ax = _low_int_0x13((0x01 << 8), 0, 0, drive, 0);
    int status = ax >> 8;
    return status;
}