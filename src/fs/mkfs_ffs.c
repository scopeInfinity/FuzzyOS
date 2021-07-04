/*
mkfs.ffs binary implementation for Linux.
*/

#include "ffs.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define FILEENTRY_LOCATION(file_id) (FS_FFS_FIRST_BLOCK_SIZE+FS_FFS_FILEENTRY_SIZE*(file_id))

void write_first_block(FILE *out) {
    union FFSMetaData block;
    strncpy(block.content.signature, FS_FFS_SIGNATURE, sizeof(block.content.signature));

    rewind(out);
    fwrite(block.bytes, 1, sizeof(block.bytes), out);
}

void write_file(int file_id, FILE *outfile, int *outfile_nextdata_block, const char *filename, FILE *srcfile) {
    fseek(srcfile, 0L, SEEK_END);
    int file_size = ftell(srcfile);
    rewind(srcfile);

    union FFSFileEntry entry;
    strncpy(entry.content.filename, filename, sizeof(entry.content.filename));
    entry.content.filesize = file_size;
    entry.content.start_block_id = (*outfile_nextdata_block);

    // Write file entry
    fseek(outfile, FILEENTRY_LOCATION(file_id), SEEK_SET);
    fwrite(entry.bytes, 1, sizeof(entry.bytes), outfile);

    // Write file content
    char buffer[512];
    size_t bytes_read;
    fseek(outfile, (*outfile_nextdata_block)*FS_BLOCK_SIZE, SEEK_SET);
    (*outfile_nextdata_block) += (file_size+FS_BLOCK_SIZE-1)/FS_BLOCK_SIZE;


    while (0 < (bytes_read = fread(buffer, 1, sizeof(buffer), srcfile))) {
        fwrite(buffer, 1, bytes_read, outfile);
    }

    printf("Added '%s' file\n", filename);
}

void write_nofile(int file_id, FILE *out) {
    union FFSFileEntry entry;
    entry.content.start_block_id = 0;
    fseek(out, FILEENTRY_LOCATION(file_id), SEEK_SET);
    fwrite(entry.bytes, 1, sizeof(entry.bytes), out);
}


int create_partition(char *src_dir, char *out_filepath) {
    printf("%s, %s\n", src_dir, out_filepath);
    FILE *src = opendir(src_dir);
    FILE *out = fopen(out_filepath, "ab");
    if (!out) {
        fprintf(stderr, "can't open output file.");
        return 1;
    }
    if (!src) {
        fprintf(stderr, "can't open src directory.");
        return 2;
    }
    // header
    write_first_block(out);
    // interate over files and write them in partition.
    struct dirent *de;
    int file_id = 0;
    int outfile_nextdata_block = FS_FFS_BLOCK_DATA_START;
    char buffer_filename[4096];
    while ((de = readdir(src)) != NULL) {
        strncpy(buffer_filename, src_dir, sizeof(buffer_filename));
        strncat(buffer_filename, "/", sizeof(buffer_filename));
        strncat(buffer_filename, de->d_name, sizeof(buffer_filename));
        if (file_id >= FS_FFS_FILEENTRY_COUNT) {
            fprintf("reached max supported files, ignoring file '%s'", buffer_filename);
            continue;
        }
        struct stat file_stat;
        stat(buffer_filename, &file_stat);
        if (!S_ISREG(file_stat.st_mode)) {
            fprintf("skipping non-regular file '%s': %d", buffer_filename, file_stat.st_mode);
            continue;
        }

        FILE *file_src = fopen(buffer_filename, "rb");
        write_file(file_id++, out, &outfile_nextdata_block, de->d_name, file_src);
        fclose(file_src);
    }
    while (file_id < FS_FFS_FILEENTRY_COUNT) {
        write_nofile(file_id++, out);
    }

    fclose(out);
    return 0;
}

int main(int argc, const char **argv) {
    if (argc != 3) {
        printf("Usage: %s <base_dir> <out_file>\n", argv[0]);
        return 0;
    }
    char *src_dir = argv[1];
    char *out_filepath = argv[2];
    return create_partition(src_dir, out_filepath);
}