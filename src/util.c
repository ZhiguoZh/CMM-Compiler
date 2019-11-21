#include "../inc/util.h"

#include <stdlib.h>
#include <stdio.h>

void* check_malloc(int sz) {
    void* ptr = malloc(sz);
    if (ptr == NULL) {
        fprintf(stderr, "Error: Run out of Memory");
        exit(1);
    }
    return ptr;
}

void check(int cond) {
    if (cond == 0) abort();
}