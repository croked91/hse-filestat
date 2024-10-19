#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

void countDirEntriesByTypes(const char* path, int* counts) {
    struct dirent* entry;
    struct stat info;
    DIR* dir = opendir(path);

    if (dir == NULL) {
        perror("Error on opening directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        char fullPath[1024];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);

        if (stat(fullPath, &info) != 0) {
            perror("Error on getting file info");
            continue;
        }

        if (S_ISDIR(info.st_mode)) {
            counts[0]++;
        } else if (S_ISREG(info.st_mode)) {
            counts[1]++;
        } else if (S_ISLNK(info.st_mode)) {
            counts[2]++;
        } else if (S_ISFIFO(info.st_mode)) {
            counts[3]++;
        } else if (S_ISSOCK(info.st_mode)) {
            counts[4]++;
        } else {
            counts[5]++;
        }
    }

    closedir(dir);
}

int main() {
    // -2 для компенсации . и ../
    int counts[6] = {-2, 0, 0, 0, 0, 0};
    const char* path = ".";

    countDirEntriesByTypes(path, counts);

    const char* types[] = {
        "directories",
        "regular files",
        "symbolic links",
        "named pipes",
        "sockets",
        "other"
    };

    for (int i = 0; i < 6; i++) {
        printf("%s: %d\n", types[i], counts[i]);
    }

    return 0;
}