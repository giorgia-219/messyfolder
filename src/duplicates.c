#include "duplicates.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4096

static int files_are_equal(const char *path1, const char *path2)
{
    FILE *file1 = fopen(path1, "rb");
    FILE *file2 = fopen(path2, "rb");

    if (file1 == NULL || file2 == NULL) {
        if (file1 != NULL) fclose(file1);
        if (file2 != NULL) fclose(file2);
        return 0;
    }

    unsigned char buffer1[BUFFER_SIZE];
    unsigned char buffer2[BUFFER_SIZE];

    size_t bytes1;
    size_t bytes2;

    do {
        bytes1 = fread(buffer1, 1, BUFFER_SIZE, file1);
        bytes2 = fread(buffer2, 1, BUFFER_SIZE, file2);

        if (bytes1 != bytes2 ||
            memcmp(buffer1, buffer2, bytes1) != 0) {

            fclose(file1);
            fclose(file2);

            return 0;
        }

    } while (bytes1 > 0);

    fclose(file1);
    fclose(file2);

    return 1;
}

int find_duplicates(FileInfo *files, int count)
{
    int duplicate_groups = 0;

    for (int i = 0; i < count; i++) {

        int found_duplicate = 0;

        for (int j = i + 1; j < count; j++) {

            /*
             * Different sizes = definitely not duplicates.
             */
            if (files[i].size != files[j].size) {
                continue;
            }

            if (files_are_equal(files[i].path, files[j].path)) {

                if (!found_duplicate) {
                    printf("\n");
                    printf(
                        "  Duplicate group:\n"
                    );

                    printf(
                        "    %s\n",
                        files[i].name
                    );

                    found_duplicate = 1;
                    duplicate_groups++;
                }

                printf(
                    "    %s\n",
                    files[j].name
                );
            }
        }
    }

    return duplicate_groups;
}