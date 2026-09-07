#include "scanner.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static const char *get_category(const char *filename)
{
    const char *extension = strrchr(filename, '.');

    if (extension == NULL) {
        return "Other";
    }

    extension++;

    if (
        strcmp(extension, "jpg") == 0 ||
        strcmp(extension, "jpeg") == 0 ||
        strcmp(extension, "png") == 0 ||
        strcmp(extension, "gif") == 0
    ) {
        return "Images";
    }

    if (
        strcmp(extension, "pdf") == 0 ||
        strcmp(extension, "doc") == 0 ||
        strcmp(extension, "docx") == 0 ||
        strcmp(extension, "txt") == 0
    ) {
        return "Documents";
    }

    if (
        strcmp(extension, "mp4") == 0 ||
        strcmp(extension, "mov") == 0 ||
        strcmp(extension, "avi") == 0
    ) {
        return "Videos";
    }

    if (
        strcmp(extension, "zip") == 0 ||
        strcmp(extension, "rar") == 0 ||
        strcmp(extension, "7z") == 0 ||
        strcmp(extension, "tar") == 0
    ) {
        return "Archives";
    }

    return "Other";
}

int scan_directory(const char *directory, FileInfo **files)
{
    DIR *dir = opendir(directory);

    if (dir == NULL) {
        perror("Could not open directory");
        return -1;
    }

    int capacity = 16;
    int count = 0;

    *files = malloc(capacity * sizeof(FileInfo));

    if (*files == NULL) {
        perror("Memory allocation failed");
        closedir(dir);
        return -1;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {

        if (
            strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0
        ) {
            continue;
        }

        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);

        struct stat file_stat;

        if (stat(path, &file_stat) != 0) {
            continue;
        }

        // Ignore directories for now
        if (!S_ISREG(file_stat.st_mode)) {
            continue;
        }

        if (count >= capacity) {
            capacity *= 2;

            FileInfo *temp = realloc(
                *files,
                capacity * sizeof(FileInfo)
            );

            if (temp == NULL) {
                perror("Memory reallocation failed");
                free(*files);
                closedir(dir);
                return -1;
            }

            *files = temp;
        }

        strncpy((*files)[count].name, entry->d_name, 255);
        (*files)[count].name[255] = '\0';

        strncpy((*files)[count].path, path, 1023);
        (*files)[count].path[1023] = '\0';

        (*files)[count].size = file_stat.st_size;

        strncpy(
            (*files)[count].category,
            get_category(entry->d_name),
            31
        );

        (*files)[count].category[31] = '\0';

        count++;
    }

    closedir(dir);

    return count;
}