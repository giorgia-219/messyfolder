#include "utils.h"

#include <stdio.h>

void format_size(long long bytes, char *buffer, int buffer_size)
{
    if (bytes < 1024) {
        snprintf(buffer, buffer_size, "%lld B", bytes);
    }
    else if (bytes < 1024 * 1024) {
        snprintf(buffer, buffer_size, "%.1f KB", bytes / 1024.0);
    }
    else if (bytes < 1024LL * 1024 * 1024) {
        snprintf(buffer, buffer_size, "%.2f MB",
                 bytes / (1024.0 * 1024.0));
    }
    else {
        snprintf(buffer, buffer_size, "%.2f GB",
                 bytes / (1024.0 * 1024.0 * 1024.0));
    }
}