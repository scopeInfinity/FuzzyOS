// written for protected mode
#include <fuzzy/kernel/process/process.h>
#include <fuzzy/memmgr/tables/gdt.h>
#include <fuzzy/memmgr/layout.h>

#include <lib/utils/logging.h>
#include <fuzzy/real_mode/client.h>

#define SECTOR_SIZE 512

static int load_sector_via_reallibrary_data(
    unsigned int dst_address,
    unsigned char drive,
    unsigned int lba,
    unsigned char count) {
    // assumes count <= 128
    int es = (MEMORY_REALLIBRARY_DATA_ADDRESS&0xF0000)>>4;
    int es_address = MEMORY_REALLIBRARY_DATA_ADDRESS&0xFFFF;
    int cylinder_head = (lba/63);
    int sector_index = lba%63 + 1;
    // https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=02h:_Read_Sectors_From_Drive
    real_mode_client(
        0x13, // interrupt number
        (0x02<<8) | count,
        es_address,
        ((cylinder_head>>2)&0xFFC0) | (sector_index),
        ((cylinder_head<<8)&0xFF00) | (drive),
        es
    );
    // https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=01h:_Get_Status_of_Last_Drive_Operation
    int eax = real_mode_client(
        0x13, // interrupt number
        (0x01<<8),
        0,
        0,
        drive,
        0
    );
    unsigned int err = eax >> 8;
    if(err) return err;

    // copy from temporary memory to real dst address.
    int dst_ds = GDT_ABS32_DS;
    int src_ds = GDT_ABS32_DS;

    kernel_memncpy_absolute(
        dst_ds,
        dst_address,
        src_ds,
        MEMORY_REALLIBRARY_DATA_ADDRESS,
        SECTOR_SIZE*(int)count);
    print_info("[load_sectors] mem %x:%x -> mem %x:%x, cnt: %d; err: %d",
        src_ds, MEMORY_REALLIBRARY_DATA_ADDRESS, dst_ds, dst_address, SECTOR_SIZE*(int)count, 0);

    return 0;
};

int load_sectors(unsigned int dst_address,
                 unsigned char drive,
                 unsigned int lba, // 0-based
                 unsigned int sector_count) {
    // load_sector_via_reallibrary_data supports upto 255 sector count
    // but the memory allocation for reallibrary data is 0x10000 which
    // amounts to 128 sectors. Please have a look at memory_layout.md
    const int OCC_MAX_SECTOR_COUNT = 128;

    int err = 0;
    while(!err && sector_count>0) {
        int occ_read_sectors = min(sector_count, OCC_MAX_SECTOR_COUNT);
        err = load_sector_via_reallibrary_data(
            dst_address,
            drive,
            lba,
            occ_read_sectors
            );
        dst_address += SECTOR_SIZE*occ_read_sectors;
        lba += occ_read_sectors;
        sector_count -= occ_read_sectors;
    }
    return err;
}
