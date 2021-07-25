// Simple Calculator
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void process() {
    char filename[]="lib.sh";

    puts("ls - list files\n");
    __asm__("NOP");
    FILE *handler = fopen(filename, "r");
    if(handler==NULL) {
        puts("File open: failed: ");
        puts(filename);
        return;
    }
    puts("File open: success: ");
    puts(filename);
    putchar('\n');

    char buffer[100];
    // line 0
    fgets(buffer, sizeof(buffer), handler);
    puts("\n0: ");
    puts(buffer);

    // line 1
    fgets(buffer, sizeof(buffer), handler);
    puts("\n1: ");
    puts(buffer);

    // line 2
    fgets(buffer, sizeof(buffer), handler);
    puts("\n2: ");
    puts(buffer);

    // line 3
    fgets(buffer, sizeof(buffer), handler);
    puts("\n3: ");
    puts(buffer);

    fclose(handler);
}

int main(int argc,char *argv[]) {
    process();
    while (1);
    return 0;
}