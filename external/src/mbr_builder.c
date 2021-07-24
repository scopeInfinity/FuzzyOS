// Classical generic MBR
// https://en.wikipedia.org/wiki/Master_boot_record#PTE
#include <fuzzy/fs/mbr.h>
#include<stdio.h>

char DRIVE = 0x80;
char DRIVE_INACTIVE = 0x00;
// https://en.wikipedia.org/wiki/Partition_type
char PARTITION_TYPE = 0x07;  // Stealing exFAT

// https://en.wikipedia.org/wiki/Cylinder-head-sector
void lba_to_chs(int lba, char chs[3]) {
    // only lower 3 bytes of CHS are valid.
    // TODO: Not implemented

    // # Sector
    // START_CHS_HEAD=
    // START_CHS_CYLINDER=
    // START_CHS_SECTOR
    // START_CHS_ADDRESS=
    // END_CHS_HEAD=
    // END_CHS_CYLINDER=
    // END_CHS_SECTORA
    // END_CHS_ADDRESS=
}

void write_boot_signature(FILE *out) {
    fseek(out, 510, SEEK_SET);
    char boot_signature[2]={0x55, 0xAA};
    fwrite(boot_signature, 1, sizeof(boot_signature), out);
}

void write_partition_entry(FILE *out, int id, char drive, int lba, int sector_count) {
    struct PartitionEntry entry;
    entry.drive = drive;
    entry.partition_type = PARTITION_TYPE;
    entry.lba = lba;
    entry.sector_count = sector_count;
    // TODO: write lba using lba_to_chs too
    fseek(out, MBR_PARTITION_BEGIN+id*sizeof(struct PartitionEntry), SEEK_SET);
    fwrite(&entry, 1, sizeof(entry), out);
    printf("Added partition entry %d at lba: %d sector_count: %d\n",
        id, lba, sector_count);
}

void write_partition(FILE *out, FILE *in, int lba) {
    int count;
    char buffer[1024];
    rewind(in);
    fseek(out, lba*512, SEEK_SET);
    while ((count = fread(buffer, sizeof(char), sizeof(buffer), in)) > 0) {
        fwrite(buffer, sizeof(char), count, out);
    }
}

void populate_outfile_using_image_prefix(FILE *out, FILE *in) {
    char buffer[1024];
    int count;
    rewind(in);
    rewind(out);
    while ((count = fread(buffer, sizeof(char), sizeof(buffer), in)) > 0) {
        fwrite(buffer, sizeof(char), count, out);
    }
}

int main(int argc, char *argv[]) {
    // Meant to create single parition entry within MBR.
    if (argc != 4) {
        printf("Usage: %s <out_file> <image_prefix> <parition1>\n", argv[0]);
        return 0;
    }
    FILE *out = fopen(argv[1], "wb");
    FILE *image_prefix = fopen(argv[2], "rb");
    FILE *part1 = fopen(argv[3], "rb");

    populate_outfile_using_image_prefix(out, image_prefix);

    int lba, sector_count;   // 4 bytes
    {
        fseek(out, 0L, SEEK_END);
        int file_size = ftell(out);
        rewind(out);
        lba=(file_size+511)/512;
    }
    {
        fseek(part1, 0L, SEEK_END);
        int file_size = ftell(part1);
        sector_count=(file_size+511)/512;
    }

    write_boot_signature(out);
    write_partition_entry(out, 0, DRIVE, lba, sector_count);
    write_partition(out, part1, lba);
    write_partition_entry(out, 1, DRIVE_INACTIVE, 0, 0);
    write_partition_entry(out, 2, DRIVE_INACTIVE, 0, 0);
    write_partition_entry(out, 3, DRIVE_INACTIVE, 0, 0);
    return 0;
}
