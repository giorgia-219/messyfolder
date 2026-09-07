#ifndef SCANNER_H
#define SCANNER_H

typedef struct {
    char name[256];
    char path[1024];
    long long size;
    char category[32];
} FileInfo;

int scan_directory(const char *directory, FileInfo **files);

#endif