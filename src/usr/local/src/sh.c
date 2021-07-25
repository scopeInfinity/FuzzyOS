// simple shell
#include <stdio.h>
#include <dirent.h>

const int COMMAND_SIZE = 200;

void banner() {
    puts("Simple Shell\n");
    puts("------------\n");
}

void handle_command(char *cmd) {
    if (strcmp(cmd, "ls")==0) {
        spawn("ls");
    } else {
        puts("error\n");
    }
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