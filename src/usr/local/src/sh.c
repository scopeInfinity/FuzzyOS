// simple shell
#include <stdio.h>
#include <process.h>
#include <string.h>

const int COMMAND_SIZE = 64;
const char COMMAND_DELIM[] = " ";
int last_status_code = 0;

void banner() {
    puts("Simple Shell\n");
    puts("------------\n");
}

int cmd_help() {
    printf("commands\n");
    printf(" > help                       print the available commands\n");
    printf(" > exit                       kill the current shell\n");
    printf(" > run ls                     print list of files\n");
    printf(" > run <filename> [arg1]...   execute available program\n");
    return 0;
}

static char *copy_arg(char *dst, char *src) {
    int slen = strlen(src);
    if (slen >= PROCESS_MAX_ARG_LEN) {
        slen = PROCESS_MAX_ARG_LEN-1;
    }
    memcpy(dst, src, slen);
    dst[slen] = NULL;
    return dst;
}

int cmd_run() {
    char *argv[PROCESS_MAX_ARGC];
    char argv_data[PROCESS_MAX_ARGC][PROCESS_MAX_ARG_LEN] = {0};

    char *token;
    int argc = 0;
    while ((token = strtok(NULL, COMMAND_DELIM)) != NULL && argc < PROCESS_MAX_ARGC-1) {
        argv[argc] = copy_arg(argv_data[argc], token);
        argc++;
    }
    argv[argc] = NULL;
    if (argc == 0) {
        printf("invalid syntax\n");
        printf("usage: run <filename> [arg1 [arg2 ...]]\n");
        return -2;
    }
    char *filename = argv[0];
    return spawnv(filename, argv);
}

int cmd_exit() {
    exit(0);
    return 0;
}

void handle_command(char *full_cmd) {
    // syntax: <cmd> [arg0]...
    //  - handle locally if <cmd> is builtin
    //  - update to err status code

    char *cmd = strtok(full_cmd, COMMAND_DELIM);

    if (strcmp(cmd, "help")==0) {
        last_status_code = cmd_help();
    } else if (strcmp(cmd, "exit")==0) {
        last_status_code = cmd_exit();
    } else if (strcmp(cmd, "run")==0) {
        last_status_code = cmd_run(cmd);
    } else {
        printf("'%s' command not found\n", cmd);
        last_status_code = 404;
    }
}

int main(int argc, char *argv[]) {
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