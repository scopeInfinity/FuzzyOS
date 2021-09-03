// list directory
#include <stdio.h>
#include <dirent.h>
#include <process.h>

void process() {
    struct DIR dir;
    opendir(&dir);
    struct dirent *dp;

    while ((dp = readdir(&dir)) !=NULL) {
        printf("%s\n", dp->d_name);
    }
}

int main(int argc,char *argv[]) {

    int p = fork();
    printf("fork() = %d\n", p);
    process();
    return 0;
}