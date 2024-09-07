#include "mem_utils.h"

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#define EXIT_FAILURE 1
#define SOFT

void *cooler_realloc(void *source, size_t new_capacity)
{
    void *res = realloc(source, new_capacity);
#ifdef SOFT
    if (NULL == res) {
        free(source);
        perror("realloc failed");
        exit(EXIT_FAILURE);
    }
#endif
    return ptr_coallesce(res, source);
}

void *cooler_malloc(size_t size)
{
    void *res = malloc(size);
#ifdef SOFT
    if (NULL == res) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
#endif
    return res;
}

#
void *stralloc(size_t size)
{
    char *res     = cooler_malloc(size);
    res[size - 1] = '\0';
    return res;
}

void *ptr_coallesce(void *a, void *b)
{
    return NULL == a ? b : a;
}
