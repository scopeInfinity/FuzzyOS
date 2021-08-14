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
    printf("commands\n");
    printf(" > help\n");
    printf(" > dir\n");
    printf(" > run <filename>\n");
    printf(" > echo <text>\n");
    printf(" > status\n");
    printf(" > exit\n");
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

int cmd_run(char *fname) {
    if(fname == NULL) {
        printf("invalid syntax\n");
        printf("usage: run <filename>\n");
        return -2;
    }
    return spawn(fname);
}

int cmd_echo(char *text) {
    if(text == NULL) {
        printf("invalid syntax\n");
        printf("usage: echo <text>\n");
        return -2;
    }
    return printf("%s\n", text);
}

int cmd_status_code() {
    printf("last cmd status code: %d\n", last_status_code);
    return 0;
}

int cmd_exit() {
    exit(0);
    return 0;
}

void handle_command(char *full_cmd) {
    // BUG
    //  program executes even if arg0 is not provided
    //  if followed by invalid instruction containing the
    //  arg0.
    //  steps to reproduce:
    //  - execute ls.out
    //  - run

    // syntax: <cmd> [arg0]
    char cmd[12];  // for now: excepts no cmd to use all bytes.
    memcpy(cmd, full_cmd, sizeof(cmd));
    char *arg0;
    {
        // <cmd>
        int i = 0;
        while(i+1<sizeof(cmd) && cmd[i]!=' ') i++;
        cmd[i++]='\0';

        // [arg0]
        if(i<sizeof(cmd)) {
            arg0 = full_cmd+i;
        } else {
            arg0 = NULL;
        }
    }

    if (strcmp(cmd, "help")==0) {
        last_status_code = cmd_help();
    } else if (strcmp(cmd, "dir")==0) {
        last_status_code = cmd_dir();
    } else if (strcmp(cmd, "run")==0) {
        last_status_code = cmd_run(arg0);
    } else if (strcmp(cmd, "echo")==0) {
        last_status_code = cmd_echo(arg0);
    } else if (strcmp(cmd, "status")==0) {
        last_status_code = cmd_status_code();
    } else if (strcmp(cmd, "exit")==0) {
        last_status_code = cmd_exit();
    } else {
        printf("'%s' command not found\n", cmd);
        last_status_code = 404;
    }
}

int main(int argc,char *argv[]) {
    char command[COMMAND_SIZE];
    banner();
    cmd_help();
    int c = 0;
    while (1) {
        puts("$ ");
        gets(command);
        handle_command(command);
    }
    return 0;
}