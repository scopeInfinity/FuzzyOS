// list directory
#include <stdio.h>
#include <dirent.h>

void process() {
    puts("list files:\n");
    struct DIR dir;
    opendir(&dir);
    struct dirent *dp;

    while ((dp = readdir(&dir)) !=NULL) {
        printf("- %s \n", dp->d_name);
    }
}

int main(int argc,char *argv[]) {
    process();
    puts("Press return key to exit...");
    getch();
    return 0;
}