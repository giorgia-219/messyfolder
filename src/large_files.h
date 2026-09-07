#ifndef LARGE_FILES_H
#define LARGE_FILES_H

#include "scanner.h"

int find_large_files(FileInfo *files, int count, long long threshold);

#endif