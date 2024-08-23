#pragma once
#include <stddef.h>

void *cooler_realloc(void *source, size_t new_capacity);

void *cooler_malloc(size_t size);