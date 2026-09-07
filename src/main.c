#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scanner.h"
#include "utils.h"
#include "organizer.h"
#include "duplicates.h"
#include "large_files.h"

#define VERSION "1.2.0"
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
        "  -v, --version    Show version information\n"
        "  -o, --organize   Organize files into category folders\n"
        "  -d, --duplicates Find duplicates\n"
        "  -l, --large <MB> Find files larger than the specified size",
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
    if (argc < 2) {
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

    int organize = 0;
    int duplicates = 0;
    int large = 0;
    long long large_threshold = 0;
    
    for (int i = 2; i < argc; i++) {

        if (strcmp(argv[i], "--organize") == 0 || strcmp(argv[i], "-o") == 0) {
            organize = 1;
        } else if (strcmp(argv[i], "--duplicates") == 0 || strcmp(argv[i], "-d") == 0) {
            duplicates = 1;
        } else if (strcmp(argv[i], "--large") == 0 || strcmp(argv[i], "-l") == 0) {
            if (i + 1 >= argc) {
                printf("Missing size for --large\n");
                return 1;
            }
            large = 1;
            large_threshold = atoll(argv[++i]) * 1024LL * 1024LL;
        } else {
            printf("Unknown option: %s\n", argv[i]);
            return 1;
        }
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

    format_size(total_size, total_buffer, sizeof(total_buffer));

    if (duplicates) {
        printf("\n");
        printf("  ─────────────────────────────────────────────────────\n");
        printf("  DUPLICATES\n");

        int groups = find_duplicates(files, count);

        if (groups == 0) {
            printf("  No duplicates found.\n");
        }
        else {
            printf("\n  Found %d duplicate groups.\n", groups);
        }
    }

    if (large) {
        printf("\n");
        printf("  LARGE FILES\n");
        printf("  ─────────────────────────────────────────────────────\n");
        printf("  Files larger than %lld MB\n\n", large_threshold / (1024LL * 1024LL));

        int large_count = find_large_files(files, count, large_threshold);

        if (large_count < 0) {
            printf("  Could not allocate memory.\n");
        }
        else if (large_count == 0) {
            printf("  No large files found.\n");
        }
        else {
            printf("\n Found %d large files.\n", large_count);
        }
    }

    if (organize) {
       int moved = organize_files(files, count);
        printf("\n  Organized %d files    %s\n", moved, total_buffer);
    } else{
        printf( "\n  Total: %d files    %s\n", count, total_buffer);
    }

    printf("\n");

        free(files);

        return 0;
    }