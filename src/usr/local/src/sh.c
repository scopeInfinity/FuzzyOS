// simple shell
#include <stdio.h>
#include <process.h>
#include <dirent.h>

const int COMMAND_SIZE = 200;
int last_status_code = 0;

void banner() {
    puts("Simple Shell\n");
    puts("------------\n");
}

void handle_command(char *cmd) {
    if (strcmp(cmd, "ls")==0) {
        last_status_code = spawn("ls");
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