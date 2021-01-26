#include "../inc/util.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

void* check_malloc(int sz) {
    void* ptr = malloc(sz);
    if (ptr == NULL) {
        fprintf(stderr, "Error: Run out of Memory");
        exit(1);
    }
    return ptr;
}

void Fatal(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}