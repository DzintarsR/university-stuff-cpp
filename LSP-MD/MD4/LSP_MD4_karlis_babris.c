#include "stdlib.h"
#include "stdio.h"
#include "sys/mman.h"
#include "unistd.h"
#include "time.h"

void res_malloc(int amount_mb) {
    int i=0;

    while(1){
        void *mem = malloc(1024*1024);

        if (amount_mb != -1  && amount_mb == i) {
            break;
        }

        if (NULL == mem) {
            break;
        }

        i++;
    }

    printf("malloc MB: %d\n", i);
}

void res_mmap(int amount_mb) {
    int i=0;

    while(1){
        void *mem;
        mem = (void*)mmap(NULL, 1024*1024, PROT_READ|PROT_WRITE, MAP_ANON|MAP_SHARED, -1, 0);

        if (amount_mb != -1  && amount_mb == i) {
            break;
        }

        if (mem == MAP_FAILED) {
            break;
        }

        i++;
    }

    printf("mmap MB: %d\n", i);
}

void res_sbrk(int amount_mb) {
    int i=0;

    while(1){
        void *mem;
        mem = sbrk(1024*1024);

        if (amount_mb != -1  && amount_mb == i) {
            break;
        }

        if (mem == (void *)-1) {
            break;
        }

        i++;
    }

    printf("sbrk MB: %d\n", i);
}


int main(int argc, char *argv[]) {
    int start = clock();

    // Test allocation
    //res_malloc(-1);
    //res_mmap(-1);
    //res_sbrk(-1);

    // Test
    //res_malloc(100);
    //res_mmap(100);
    //res_sbrk(100);

    // Spent time
    printf("Spent time: %f\n", ((float)(clock() - start))/CLOCKS_PER_SEC);
    return 0;
}
