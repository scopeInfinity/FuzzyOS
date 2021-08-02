// list directory
#include <stdio.h>
#include <string.h>

void process(char filename[]) {
    puts("file: ");
    puts(filename);
    putchar('\n');

    FILE *handler = fopen(filename, "r");
    if(handler==NULL) {
        puts("file open: failed: ");
        puts(filename);
        return;
    }
    puts("content: \n");
    char buffer[80];
    for (size_t i = 0; i < 20; i++) {
        if (!fgets(buffer, sizeof(buffer), handler)) {
            break;
        }
        puts(buffer);
        putchar('\n');
    }
    fclose(handler);
}

int main(int argc,char *argv[]) {
    char filename[] = "lib.sh";
    process(filename);
    while (1);
    return 0;
}