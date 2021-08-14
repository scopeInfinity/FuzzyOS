// simple shell
#include <stdio.h>
#include <process.h>
#include <dirent.h>
#include <string.h>

const int COMMAND_SIZE = 200;
int last_status_code = 0;

void banner() {
    puts("Simple Shell\n");
    puts("------------\n");
}

int cmd_help() {
    // dir
    printf("allowed expression\n");
    printf("  $ <cmd> [arg0]\n");
    printf("\n");
    printf("defined <cmd>:\n");
    printf("   - help\n");
    printf("   - dir\n");
    return 0;
}

int cmd_dir() {
    struct DIR dir;
    opendir(&dir);
    struct dirent *dp;

    while ((dp = readdir(&dir)) !=NULL) {
        printf("%s \n", dp->d_name);
    }
    return 0;
}

void handle_command(char *full_cmd) {
    // syntax: <cmd> [arg0]
    char cmd[12];  // for now: excepts no cmd to use all bytes.
    memcpy(cmd, full_cmd, sizeof(cmd));
    char *arg0 = NULL;
    {
        // <cmd>
        int i = 0;
        while(i+1<sizeof(cmd) && cmd[i]!=' ') i++;
        cmd[i++]='\0';

        // [arg0]
        if(i<sizeof(cmd)) {
            arg0 = full_cmd+i;
        }
    }

    if (strcmp(cmd, "help")==0) {
        last_status_code = cmd_help();
    } else if (strcmp(cmd, "dir")==0) {
        last_status_code = cmd_dir();
    } else {
        printf("'%s' command not found\n", cmd);
        last_status_code = 404;
    }
    printf("status code: %d\n", last_status_code);
}

int main(int argc,char *argv[]) {
    char command[COMMAND_SIZE];
    banner();
    int c = 0;
    while (1) {
        puts("$ ");
        gets(command);
        handle_command(command);
    }
    return 0;
}