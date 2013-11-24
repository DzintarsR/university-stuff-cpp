#include "stdio.h"
#include "stdlib.h"

struct chunksList {
    int size;
    int used;

    struct chunksList *next;
    struct chunksList *prev;
};

typedef struct chunksList chunksList_t;

chunksList_t *firstChunk = NULL;
chunksList_t *lastChunk;

void locateMemory(FILE *chunksFile);
float fragmentation();

void bestFit(FILE *sizesFile);
void worstFit(FILE *sizesFile);
void firstFit(FILE *sizesFile);
void nextFit(FILE *sizesFile);

int main(int argc, char *argv[]) {
    FILE *chunks;
    FILE *sizes;

    int chunks_place = 2;
    int sizes_place = 4;

    /* Check if input values were given */
    if (argc < 4) {
        printf("Specify program with input files: ./main -c chunks -s sizes\n");
        return 0;
    }

    if (strcmp("-c", argv[3], 2) == 0) {
        chunks_place = 4;
        sizes_place = 2;
    }

    chunks = fopen(argv[chunks_place], "r");
	sizes = fopen(argv[sizes_place], "r");

    /* Check if chunks file exists */
	if (chunks == NULL) {
        printf("Couldn't open %s file\n", argv[chunks_place]);
        return 0;
	}

    /* Check if sizes file exists */
	if (sizes == NULL) {
	    printf("Couldn't open %s file\n", argv[sizes_place]);
        return 0;
	}

    /* Test BestFit algorithm */
	locateMemory(chunks);
	bestFit(sizes);

	/* Test WorstFit algorithm */
	locateMemory(chunks);
	worstFit(sizes);

	/* Test FirstFit algorithm */
	locateMemory(chunks);
	firstFit(sizes);

	/* Test NextFit algorithm */
	locateMemory(chunks);
	nextFit(sizes);

    fclose(chunks);
    fclose(sizes);

	return 0;
}

void bestFit(FILE *sizesFile) {
    /* code here */
}

void worstFit(FILE *sizesFile) {
    /* code here */
}

void firstFit(FILE *sizesFile) {
    int value;
    int unallocated = 0;
    chunksList_t *p;

    while (fscanf(sizesFile, "%d", &value) != EOF) {
        p = firstChunk;

        while (p != NULL) {
            if ((p->size - p->used) >= value) {
                p->used += value;
                break;
            }

            p = p->next;
        }

        if (p == NULL) {
            unallocated += value;
        }
    }

    p = firstChunk;
    while (p != NULL) {
        printf("size: %d \t used: %d\n", p->size, p->used);
        p = p->next;
    }

    printf("unallocated amount %d\n", unallocated);
    printf("fragmentation: %f\n", fragmentation());
}

void nextFit(FILE *sizesFile) {
    /* code here */
}

void locateMemory(FILE *chunksFile) {
    if (firstChunk == NULL) {
        /* Create new two way linked list with chunks file values */
        int value;
        firstChunk = malloc(sizeof(struct chunksList));
        firstChunk->prev = NULL;
        firstChunk->next = NULL;

        chunksList_t *p = firstChunk;
        chunksList_t *p_prev;

        while (fscanf(chunksFile, "%d", &value) != EOF) {
            p->used = 0;
            p->size = value;
            p->prev = NULL;
            p->next = NULL;

            if (p != firstChunk) {
                p->prev = p_prev;
                p_prev->next = p;
            }

            p_prev = p;
            p = malloc(sizeof(struct chunksList));
        }

        lastChunk = p_prev;
    } else {
        /* Reset chunks for reuse */
        chunksList_t *p = firstChunk;
        while (p != NULL) {
            p->used = 0;
            p = p->next;
        }
    }
}

float fragmentation() {
    /*
    Algorithm: http://en.wikipedia.org/wiki/Fragmentation_%28computer%29
    fragmentation = 1 - (largest_free_memory_block / total_free_memory)
    returned: fragmentation * 100 (percent value of fragmentation)
    */

    int largest_free = 0;
    int free_blocks = 0;
    chunksList_t *p = firstChunk;

    while (p != NULL) {
        int free = p->size - p->used;

        free_blocks += free;
        if (free > largest_free) {
            largest_free = free;
        }

        p = p->next;
    }

    return (1 - ((float)largest_free / free_blocks)) * 100;
}
