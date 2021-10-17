// simple shell
#include <process.h>
#include <stdio.h>
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
    printf(" > <filename> [arg1]...       execute executable program\n");
    printf(" > ls -h                      example to show ls usage\n");
    last_status_code = 0;
    return 0;
}

static char *copy_arg(char *dst, char *src) {
    int slen = strlen(src);
    if (slen >= PROCESS_MAX_ARG_LEN) {
        slen = PROCESS_MAX_ARG_LEN - 1;
    }
    memcpy(dst, src, slen);
    dst[slen] = NULL;
    return dst;
}

int cmd_run(char *cmd) {
    if (cmd == NULL) {
        // no command entered
        last_status_code = 0;
        return 0;
    }
    char *argv[PROCESS_MAX_ARGC];
    char argv_data[PROCESS_MAX_ARGC][PROCESS_MAX_ARG_LEN] = {0};

    char *token;
    int argc = 0;
    argv[argc] = copy_arg(argv_data[argc], cmd); // executable filename
    argc++;
    while ((token = strtok(NULL, COMMAND_DELIM)) != NULL &&
           argc < PROCESS_MAX_ARGC - 1) {
        argv[argc] = copy_arg(argv_data[argc], token);
        argc++;
    }
    argv[argc] = NULL;

    char *filename = argv[0];
    int pid = spawnv(filename, argv);
    if (pid < 0) {
        // failed
        return pid;
    }
    waitpid(pid, &last_status_code);
    return 0;
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

    if (strcmp(cmd, "help") == 0) {
        cmd_help();
    } else if (strcmp(cmd, "exit") == 0) {
        cmd_exit();
    } else {
        if (cmd_run(cmd) < 0) {
            printf("failed to run '%s' command\n", cmd);
        }
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