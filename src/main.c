#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scanner.h"
#include "utils.h"

#define VERSION "0.2.0"
#define NAME_WIDTH 32
#define CATEGORY_COUNT 5

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

typedef struct {
    int count;
    long long total_size;
} CategorySummary;

const char *categories[CATEGORY_COUNT] = {
    "Images",
    "Documents",
    "Videos",
    "Archives",
    "Other"
};

static int get_category_index(const char *category)
{
    for (int i = 0; i < CATEGORY_COUNT; i++) {
        if (strcmp(category, categories[i]) == 0) {
            return i;
        }
    }

    return CATEGORY_COUNT - 1;
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

    CategorySummary summary[CATEGORY_COUNT] = {0};

    for (int i = 0; i < count; i++) {
    int category_index = get_category_index(files[i].category);

    summary[category_index].count++;
    summary[category_index].total_size += files[i].size;
    }

    long long total_size = 0;
    for (int i = 0; i < count; i++) {
        total_size += files[i].size;
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

    printf("  ─────────────────────────────────────────────────────\n");
    printf("  SUMMARY\n\n");

    for (int i = 0; i < CATEGORY_COUNT; i++) {

        char size_buffer[32];

        format_size(
            summary[i].total_size,
            size_buffer,
            sizeof(size_buffer)
        );

        printf(
            "  %-12s %5d files    %10s\n",
            categories[i],
            summary[i].count,
            size_buffer
        );
    }

    char total_buffer[32];

    format_size(
        total_size,
        total_buffer,
        sizeof(total_buffer)
    );

    printf(
        "\n  Total: %d files    %s\n",
        count,
        total_buffer
    );

    printf("\n");

        free(files);

        return 0;
    }