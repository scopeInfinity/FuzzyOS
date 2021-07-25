// list directory
#include <stdio.h>
#include <dirent.h>

void process() {
    puts("list files:\n");
    struct DIR dir;
    opendir(&dir);
    struct dirent *dp;

    while ((dp = readdir(&dir)) !=NULL) {
        puts(dp->d_name);
        puts("\n");
    }
}

int main(int argc,char *argv[]) {
    process();
    while (1);
    return 0;
}