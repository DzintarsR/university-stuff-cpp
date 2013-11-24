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

void prepareEnvironment(FILE *chunksFile, FILE *sizesFile);
void printData(int unallocated, char *name);
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
	prepareEnvironment(chunks, sizes);
	bestFit(sizes);

	/* Test WorstFit algorithm */
	prepareEnvironment(chunks, sizes);
	worstFit(sizes);

	/* Test FirstFit algorithm */
	prepareEnvironment(chunks, sizes);
	firstFit(sizes);

	/* Test NextFit algorithm */
	prepareEnvironment(chunks, sizes);
	nextFit(sizes);

    fclose(chunks);
    fclose(sizes);

	return 0;
}

void bestFit(FILE *sizesFile) {
    int value, free, unallocated = 0;
    chunksList_t *p, *possible_place = NULL;

    while (fscanf(sizesFile, "%d", &value) != EOF) {
        p = firstChunk;

        while (p != NULL) {
            free = p->size - p->used;

            if (free >= value) {
                if (free == value) {
                    p->used += value;
                    possible_place = NULL;
                    break;
                } else {
                    if (possible_place != NULL) {
                        if (free < (possible_place->size - possible_place->used)) {
                            possible_place = p;
                        }
                    } else {
                        possible_place = p;
                    }
                }
            }

            p = p->next;
        }

        if (p == NULL && possible_place == NULL) {
            unallocated += value;
        }

        if (possible_place != NULL) {
            possible_place->used += value;
            possible_place = NULL;
        }
    }

    printData(unallocated, "BestFit");
}

void worstFit(FILE *sizesFile) {
    int value, free, unallocated = 0;
    chunksList_t *p, *possible_place = NULL;

    while (fscanf(sizesFile, "%d", &value) != EOF) {
        p = firstChunk;

        while (p != NULL) {
            free = p->size - p->used;

            if (free >= value) {
                if (possible_place != NULL) {
                    if (free > (possible_place->size - possible_place->used)) {
                        possible_place = p;
                    }
                } else {
                    possible_place = p;
                }
            }

            p = p->next;
        }

        if (p == NULL && possible_place == NULL) {
            unallocated += value;
        }

        if (possible_place != NULL) {
            possible_place->used += value;
            possible_place = NULL;
        }
    }

    printData(unallocated, "WorstFit");
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

    printData(unallocated, "FirstFit");
}

void nextFit(FILE *sizesFile) {
    int value;
    int unallocated = 0;
    chunksList_t *start, *p;

    start = firstChunk;
    lastChunk->next = firstChunk;

    while (fscanf(sizesFile, "%d", &value) != EOF) {
        p = NULL;

        while (p == NULL || p != start) {
            if (p == NULL) {
                p = start;
            }

            if ((p->size - p->used) >= value) {
                p->used += value;
                start = p;
                p = NULL;
                break;
            }

            p = p->next;
        }

        if (p == start) {
            unallocated += value;
        }
    }

    lastChunk->next = NULL;
    printData(unallocated, "NextFit");
}

void prepareEnvironment(FILE *chunksFile, FILE *sizesFile) {
    /* Reset sizes file pointer to start */
    fseek(sizesFile, 0, SEEK_SET);

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

        /* Reset to default */
        firstChunk->prev = NULL;
        lastChunk->next = NULL;
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

    /* If all memory has been allocated */
    if (free_blocks == 0) {
        return -1;
    }

    return (1 - ((float)largest_free / free_blocks)) * 100;
}

void printData(int unallocated, char *name) {
    chunksList_t *p = firstChunk;

    printf("===== START %s =====\n", name);

    while (p != NULL) {
        printf("size: %d \t used: %d\n", p->size, p->used);
        p = p->next;
    }

    printf("unallocated amount %d\n", unallocated);
    printf("fragmentation: %f\n", fragmentation());

    printf("===== END %s =====\n", name);
}
