// simple shell
#include <stdio.h>
#include <process.h>
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
    printf(" > run ls\n");
    printf(" > run <filename>\n");
    printf(" > echo <text>\n");
    printf(" > status\n");
    printf(" > exit\n");
    return 0;
}

int cmd_run(char *fname) {
    if(fname == NULL) {
        printf("invalid syntax\n");
        printf("usage: run <filename>\n");
        return -2;
    }
    return spawnl(fname, fname, NULL);
}

int cmd_echo(char *text) {
    if(text == NULL) {
        printf("invalid syntax\n");
        printf("usage: echo <text>\n");
        return -2;
    }
    return printf("%s\n", text);
}

int cmd_exit() {
    exit(0);
    return 0;
}

void handle_command(char *full_cmd) {
    // syntax: <cmd> [arg0]
    char cmd[12];  // for now: excepts no cmd to use all bytes.
    memcpy(cmd, full_cmd, sizeof(cmd));
    char *arg0;
    {
        // <cmd>
        int i = 0;
        while(i+1<sizeof(cmd) && cmd[i]!=' ' && cmd[i]!='\0') i++;
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
    } else if (strcmp(cmd, "run")==0) {
        last_status_code = cmd_run(arg0);
    } else if (strcmp(cmd, "echo")==0) {
        last_status_code = cmd_echo(arg0);
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
        printf("[%d] $ ", last_status_code);
        gets(command);
        handle_command(command);
    }
    return 0;
}