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
            //pDir = opendir(argv[2]);
            printf("-> %s\n", name);
            match_dir_file(opendir(pDirent->d_name), name, dir);
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

    //while((pDirent = readdir(pDir)) != NULL) {
    //    if (pDirent->d_type != DT_DIR) {
    //        if (strncmp(argv[1], pDirent->d_name, 255) == 0) {
    //            printf("%s/%s\n", argv[2], argv[1]);
    //        }
    //    }
    //}
    closedir(pDir);

    return 0;
}
