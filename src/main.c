#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scanner.h"
#include "utils.h"

#define VERSION "0.1.0"
#define NAME_WIDTH 32

static void print_help(void)
{
    printf(
        "MessyFolder v%s\n\n"
        "Usage:\n"
        "  messyfolder <directory>\n\n"
        "Options:\n"
        "  -h, --help       Show this help message\n"
        "  -v, --version    Show version information\n",
        VERSION
    );
}

static void print_version(void)
{
    printf("MessyFolder v%s\n", VERSION);
}

static void print_filename(const char *name)
{
    size_t length = strlen(name);

    if (length <= NAME_WIDTH) {
        printf("%-*s", NAME_WIDTH, name);
        return;
    }

    /*
     * Keep the file extension visible when possible.
     *
     * Example:
     * very-long-file-name-that-is-too-long.pdf
     * becomes:
     * very-long-file-name-that-is...pdf
     */

    const char *extension = strrchr(name, '.');

    if (extension != NULL && strlen(extension) < NAME_WIDTH - 4) {
        int extension_length = strlen(extension);
        int prefix_length = NAME_WIDTH - extension_length - 3;

        printf("%.*s...%s", prefix_length, name, extension);
    }
    else {
        printf("%.*s...", NAME_WIDTH - 3, name);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        print_help();
        return 1;
    }

    if (
        strcmp(argv[1], "-h") == 0 ||
        strcmp(argv[1], "--help") == 0
    ) {
        print_help();
        return 0;
    }

    if (
        strcmp(argv[1], "-v") == 0 ||
        strcmp(argv[1], "--version") == 0
    ) {
        print_version();
        return 0;
    }

    FileInfo *files = NULL;

    int count = scan_directory(argv[1], &files);

    if (count < 0) {
        return 1;
    }

    printf("\n");
    printf("  MessyFolder v%s\n", VERSION);
    printf("  ─────────────────────────────────────────────────────\n");
    printf("  Directory: %s\n", argv[1]);
    printf("  Files found: %d\n\n", count);

    printf(
        "  %-32s %-12s %12s\n",
        "Name",
        "Category",
        "Size"
    );

    printf(
        "  %-32s %-12s %12s\n",
        "--------------------------------",
        "------------",
        "------------"
    );

    for (int i = 0; i < count; i++) {

        char size_buffer[32];

        format_size(
            files[i].size,
            size_buffer,
            sizeof(size_buffer)
        );

        printf("  ");
        print_filename(files[i].name);

        printf(
            " %-12s %12s\n",
            files[i].category,
            size_buffer
        );
    }

    printf("\n");

    free(files);

    return 0;
}