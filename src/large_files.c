#include "large_files.h"

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

static int compare_files(const void *a, const void *b)
{
    const FileInfo *file_a = a;
    const FileInfo *file_b = b;

    if (file_a->size < file_b->size) {
        return 1;
    }

    if (file_a->size > file_b->size) {
        return -1;
    }

    return 0;
}

int find_large_files(FileInfo *files, int count, long long threshold)
{
    FileInfo *large_files = malloc(count * sizeof(FileInfo));

    if (large_files == NULL) {
        return -1;
    }

    int large_count = 0;

    for (int i = 0; i < count; i++) {
        if (files[i].size >= threshold) {
            large_files[large_count++] = files[i];
        }
    }

    qsort(
        large_files,
        large_count,
        sizeof(FileInfo),
        compare_files
    );

    for (int i = 0; i < large_count; i++) {

        char size_buffer[32];

        format_size(
            large_files[i].size,
            size_buffer,
            sizeof(size_buffer)
        );

        printf(
            "  %-40s %10s\n",
            large_files[i].name,
            size_buffer
        );
    }

    free(large_files);

    return large_count;
}