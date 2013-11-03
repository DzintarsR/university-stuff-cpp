#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void f() {
    char array[100 * 1024 * 1024];
    memset(array, 0, sizeof(array));
}

void stack_alloc() {
    int i;
    for (i=0; i<10000; i++) {
        void *alloca(size_t size);
    }
}

int main(void)
{
        printf("allocating...\n");

        /*
        void *m = malloc(1024*1024*1024);

        int i;
        void *mas[1000000];

        for (i=0; i<1000000; i++) {
            void *m = malloc(1024*1024);
            mas[i] = m;
        }
        */

        //f();

        stack_alloc();

        printf("done\n");
        return 0;
}
