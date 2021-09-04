#pragma once

int load_sectors(unsigned int full_address,
                 unsigned char drive,
                 unsigned int lba, // 0-based
                 unsigned int count);