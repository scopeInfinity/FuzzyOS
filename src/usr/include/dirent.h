#pragma once

#define FILENAME_LIMIT 120  // same as fuzzy/fs/ffs.h

struct dirent {
    // In FFS the only file types are only normal file.
    char d_name[FILENAME_LIMIT];
};

struct DIR {
    struct dirent entry;
    int kernel_file_id;  // iterator
};

// There is only one global directory in FFS.
void opendir(struct DIR *dirp);
struct dirent *readdir(struct DIR *dirp);
