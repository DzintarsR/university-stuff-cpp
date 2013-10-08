#include "stdio.h"
#include "string.h"
#include "dirent.h"

void match_dir_file(DIR *pDir, char *name, char *dir) {
    struct dirent *pDirent;

    while(pDir != NULL && ((pDirent = readdir(pDir)) != NULL)) {
        if (pDirent->d_type != DT_DIR) {
            if (strncmp(name, pDirent->d_name, 255) == 0) {
                printf("%s/%s\n", dir, name);
            }
        } else {
            if (strncmp(pDirent->d_name, "..", 2) != 0 && strncmp(pDirent->d_name, ".", 1) != 0) {
                printf("-> %s\n", pDirent->d_name);
                match_dir_file(opendir(pDirent->d_name), name, dir);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    struct dirent *pDirent;
    DIR *pDir;

    if (argc < 2) {
        return -1;
    }

    pDir = opendir(argv[2]);

    if (pDir == NULL) {
        printf("Unexisting directory: %s\n", argv[2]);
        return 1;
    }

    match_dir_file(pDir, argv[1], argv[2]);

    closedir(pDir);

    return 0;
}
