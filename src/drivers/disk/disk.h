#pragma once

int load_sectors(unsigned int address,
                 unsigned char drive,
                 unsigned int sector_index, // 1-based
                 unsigned char count);