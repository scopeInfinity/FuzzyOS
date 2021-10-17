#include <dirent.h>
#include <stdio.h>
#include <sys/syscall.h>

void opendir(struct DIR *dirp) { dirp->kernel_file_id = -1; }

struct dirent *readdir(struct DIR *dirp) {
    dirp->kernel_file_id =
        SYSCALL_A3(SYSCALL_FILE_OP, SYSCALL_FILE_SUB_READ_DIR,
                   dirp->kernel_file_id, &dirp->entry);
    if (dirp->kernel_file_id == -1) {
        return NULL;
    }
    return &dirp->entry;
}
