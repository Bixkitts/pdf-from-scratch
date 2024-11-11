#pragma once
#include <stddef.h>

void *ptr_coallesce(void *a, void *b);

void *cooler_realloc(void *source, size_t new_capacity);

void *cooler_malloc(size_t size);

void *cooler_calloc(size_t num, size_t size);

void *stralloc(size_t size);
