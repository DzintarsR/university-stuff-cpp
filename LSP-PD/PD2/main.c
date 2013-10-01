#include "stdio.h"
#include "stdlib.h"

char buffer[32001];
char confirm[1];

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return -1;
    }

    // Check if write file exists
    FILE *w_exists = fopen(argv[2], "rb");
    if (w_exists != NULL) {
        printf("Rewrite existing %s file? (y/n)\n", argv[2]);
        gets(confirm);

        if (confirm[0] != 'y') {
            return -1;
        }
    }

    // Atver failu lasīšanai
    FILE *r_file = fopen(argv[1], "rb");
    FILE *w_file = fopen(argv[2], "w+");

    // Check if read file exists
    if (r_file == NULL) {
        printf("No read file\n");
        return -1;
    }

    // Check if could create write file
    if (w_file == NULL) {
        printf("No write file\n");
        return -1;
    }

    while(1) {
        size_t count = fread(buffer, sizeof(char), 32000, r_file);

        fputs(buffer, w_file);
        fflush(w_file);

        if (count < 32000) {
            break;
        }
    }

    fclose(r_file);
    fclose(w_file);
    return 0;
}
