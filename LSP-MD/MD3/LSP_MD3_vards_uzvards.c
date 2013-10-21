#include "stdio.h"
#include "string.h"
#include "dirent.h"
#include "stdlib.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#define NAME_SIZE 255
#define DIR_SIZE 1000
#define DATE_SIZE 255
#define BASE_DIR "."

struct File {
    char name[NAME_SIZE];
    char dir[DIR_SIZE];
    char date[DATE_SIZE];
    long long size;

    struct File *match;
    struct File *next;
};

typedef struct File File_t;

int name_length(char *name) {
    int i=0;
    while (1) {
        if (name[i] == '\0') {
            return  i;
        }
        i++;
    }
}

void strip_break(char *name) {
    int i=0;
    while (1) {
        if (name[i] == '\n') {
            name[i] = '\0';
            break;
        }

        if (name[i] == '\0') {
            break;
        }

        i++;
    }
}

void append_file(struct File *first_file, struct File *new_file, int type) {
    while (first_file != NULL) {
        int matched_default = 0;
        int matched_time = 0;
        int matched_md5 = 0;

        // By name, size
        if (type == 1) {
            matched_time = 1;
            matched_md5 = 1;
        }

        // By name, size, date
        if (type == 2) {
            matched_md5 = 1;
        }

        // By md5
        if (type == 3) {
            matched_default = 1;
            matched_time = 1;
        }

        if (type == 1 || type == 2) {
            if (strncmp(first_file->name, new_file->name, name_length(new_file->name)) == 0 && first_file->size == new_file->size) {
                matched_default = 1;
            }
        }

        if (type == 2) {
            if (strncmp(first_file->date, new_file->date, name_length(new_file->date)) == 0) {
                matched_time = 1;
            }
        }

        if (matched_default && matched_time && matched_md5) {
            if (first_file->match == NULL) {
                first_file->match = new_file;
                break;
            } else {
                File_t *fm = first_file->match;
                while (fm != NULL) {
                    if (fm->match == NULL) {
                        fm->match = new_file;
                        break;
                    }
                    fm = fm->match;
                }
                break;
            }
        }


        if (first_file->next == NULL) {
            first_file->next = new_file;
            break;
        }

        first_file = first_file->next;
    }
}

void match_dir_file(DIR *pDir, char *dir, struct File *file, int type) {
    struct dirent *pDirent;

    if (pDir == NULL) {
        return;
    }

    while((pDirent = readdir(pDir)) != NULL) {
        struct stat p_statbuf;

        char filename[DIR_SIZE];
        strcpy(filename, dir);
        strcat(filename, "/");
        strcat(filename, pDirent->d_name);

        lstat(filename, &p_statbuf);

        // Ignore soft links
        if (S_ISLNK(p_statbuf.st_mode)) {
            continue;
        }

        if (pDirent->d_type != DT_DIR) {
            File_t *f = malloc(sizeof(struct File));
            f->next = NULL;
            f->match = NULL;
            f->size = p_statbuf.st_size;

            strcpy(f->name, pDirent->d_name);
            strcpy(f->dir, dir);
            strcpy(f->date, ctime(&p_statbuf.st_ctime));
            strip_break(f->date);

            append_file(file, f, type);
        } else {
            if (strncmp(pDirent->d_name, "..", 2) != 0 && strncmp(pDirent->d_name, ".", 1) != 0) {
                DIR *tDir;
                tDir = opendir(filename);
                match_dir_file(tDir, filename, file, type);
                closedir(tDir);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    struct dirent *pDirent;
    DIR *pDir;
    pDir = opendir(BASE_DIR);

    File_t *first = malloc(sizeof(struct File));
    first->next = NULL;
    first->match = NULL;

    int i;
    int type = 1;
    int is_help = 0;

    for (i=1; i<argc; i++) {
        if (strncmp(argv[i], "-d", 2) == 0) {
            type = 2;
        }

        else if (strncmp(argv[i], "--help", 6) == 0) {
            is_help = 1;
        }
    }

    if (is_help) {
        printf("Usage: ./md3 [options]\n");
        printf(" --help \t usage information\n");
        printf(" -d \t\t match by date\n");
        printf(" -md5, --md5 \t match by content md5\n");

        return 0;
    }

    match_dir_file(pDir, BASE_DIR, first, type);

    while (first != NULL) {
        if (first->match != NULL) {
            printf("=== %s %lld %s\n", first->date, first->size, first->name);
            File_t *f = first;
            while (f != NULL) {
                printf("%s/%s\n", f->dir, f->name);
                f = f->match;
            }
            printf("...\n");
        }

        first = first->next;
    }

    closedir(pDir);

    return 0;
}
