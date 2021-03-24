#ifndef COMMON_H
#define COMMON_H

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

#define MIN(x, y) ((x) <= (y) ? (x) : (y))
#define MAX(x, y) ((x) >= (y) ? (x) : (y))
#define CLAMP_MAX(x, max) MIN(x, max)
#define CLAMP_MIN(x, min) MAX(x, min)

void *xmalloc(size_t len) {
    void *ret = malloc(len);
    if (!ret) {
        perror("xmalloc");
        exit(EXIT_FAILURE);
    }

    return ret;
}

void *xrealloc(void *source, size_t len) {
    void *ret = realloc(source, len);
    if (!ret) {
        perror("xrealloc");
        exit(EXIT_FAILURE);
    }

    return ret;
}

void *xcalloc(size_t num_elems, size_t elem_size) {
    void *ret = calloc(num_elems, elem_size);
    if (!ret) {
        perror("xcalloc");
        exit(EXIT_FAILURE);
    }

    return ret;
}

#endif
