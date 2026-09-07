#include "organizer.h"

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

static int create_directory(const char *path)
{
    if (mkdir(path, 0755) == 0) {
        return 0;
    }

    if (errno == EEXIST) {
        return 0;
    }

    perror("Could not create directory");
    return -1;
}

static int move_file(const char *source, const char *destination)
{
    if (rename(source, destination) != 0) {
        perror("Could not move file");
        return -1;
    }

    return 0;
}

int organize_files(FileInfo *files, int count)
{
    int moved = 0;

    for (int i = 0; i < count; i++) {

        const char *last_slash = strrchr(files[i].path, '/');

        if (last_slash == NULL) {
            continue;
        }

        int directory_length = last_slash - files[i].path;

        char directory[1024];
        char destination[1024];

        snprintf(
            directory,
            sizeof(directory),
            "%.*s/%s",
            directory_length,
            files[i].path,
            files[i].category
        );

        snprintf(
            destination,
            sizeof(destination),
            "%s/%s",
            directory,
            files[i].name
        );

        if (create_directory(directory) != 0) {
            continue;
        }

        if (move_file(files[i].path, destination) == 0) {
            moved++;
        }
    }

    return moved;
}