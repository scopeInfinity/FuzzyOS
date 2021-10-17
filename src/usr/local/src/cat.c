// print content of file
#include <stdio.h>
#include <string.h>

void process(char filename[]) {
    FILE *handler = fopen(filename, "r");
    if (handler == NULL) {
        printf("failed to open '%s' file.\n", filename);
        return;
    }
    char buffer[80];
    while (1) {
        if (!fgets(buffer, sizeof(buffer), handler)) {
            int err = ferror(handler);
            if (err) {
                printf("Error: failed to read next chunk, code %d\n", err);
                exit(err);
            }
            break;
        }
        puts(buffer);
    }
    fclose(handler);
}

int print_usage() {
    printf("Usage: cat <filename>\n");
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return print_usage();
    }
    char *filename = argv[1];
    process(filename);
    return 0;
}