// Simple Calculator
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char *argv[]) {
    puts("ls - list files\n");
    FILE *handler = fopen("lib.sh", "r");
    if(handler!=NULL) {
        puts("FILE open: success.");
    } else {
        puts("FILE open: failed.");
    }
    fclose(handler);
    while (1);

    return 0;
}