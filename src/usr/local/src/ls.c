// list directory
#include <dirent.h>
#include <stdio.h>

void process(int is_long_listing) {
    struct DIR dir;
    opendir(&dir);
    struct dirent *dp;

    while ((dp = readdir(&dir)) != NULL) {
        if (is_long_listing) {
            printf("r-%c  %d  %s\n", (dp->flag & DIRENT_EXECUTABLE) ? 'x' : '-',
                   dp->size, dp->d_name);
        } else {
            printf("%s\n", dp->d_name);
        }
    }
}

int print_usage() {
    printf("Usage: ls [-h] [-l]\n");
    return 0;
}

int main(int argc, char *argv[]) {
    int is_long_listing = 0;

    // command line argument parsing
    for (size_t i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            print_usage();
            return 0;
        } else if (strcmp(argv[i], "-l") == 0) {
            is_long_listing = 1;
        } else {
            printf("unrecognized flag: '%s'\n", argv[i]);
            print_usage();
            return 1;
        }
    }

    process(is_long_listing);
    return 0;
}