// list directory
#include <stdio.h>
#include <string.h>

void process(char filename[]) {
    printf("file: %s\n", filename);

    FILE *handler = fopen(filename, "r");
    if(handler==NULL) {
        printf("file open: failed: %s", filename);
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