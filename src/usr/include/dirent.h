#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
namespace std {
#endif

#define FILENAME_LIMIT 100 // same as fuzzy/fs/ffs.h

enum dirent_flag { DIRENT_EXECUTABLE = 1 << 0 };

struct dirent {
    // In FFS the only file types are only normal file.
    char d_name[FILENAME_LIMIT];
    uint32_t size;
    uint32_t flag;
};

struct DIR {
    struct dirent entry;
    int kernel_file_id; // iterator
};

// There is only one global directory in FFS.
void opendir(struct DIR *dirp);
struct dirent *readdir(struct DIR *dirp);

#ifdef __cplusplus
} // namespace std end
} // extern C end
#endif
