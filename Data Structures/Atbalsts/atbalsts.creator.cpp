#include "string.h"
#include "stdio.h"
#include "stdlib.h"

int main() {
    FILE* outputFile;
    outputFile = fopen("atbalsts.in", "w+");
    int n=1;
    fprintf(outputFile, "15000 15000\n");
    while (n!=5000) {
        fprintf(outputFile, "R %d\n", 12);
        n++;
    }

    n=1;
    while(n!=5000) {
        fprintf(outputFile, "M %d\n", n*5);
        n++;
    }

    fclose(outputFile);
    return 0;
}
