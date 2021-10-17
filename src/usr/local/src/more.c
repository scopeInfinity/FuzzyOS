// buffered print content of file
#include <stdio.h>
#include <string.h>

// assumption
#define SCREEN_WIDTH 0x50
#define SCREEN_HEIGHT 0x19

void process(char filename[]) {
    FILE *handler = fopen(filename, "r");
    if (handler == NULL) {
        printf("failed to open '%s' file.\n", filename);
        return;
    }
    char buffer[SCREEN_WIDTH + 1];
    int count_before_buffer = SCREEN_HEIGHT - 2;
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
        if (count_before_buffer > 0) {
            count_before_buffer--;
        } else {
            // buffer
            char c = getch();
            if (c == 'q' || c == 'Q') {
                exit(0);
            }
            // continue
        }
    }
    fclose(handler);
}

int print_usage() {
    printf("Usage: more <filename>\n");
    printf(" > buffer will start when output takes over the whole screen\n");
    printf(" > press 'q' to exit\n");
    printf(" > press any other key to scroll down\n");
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