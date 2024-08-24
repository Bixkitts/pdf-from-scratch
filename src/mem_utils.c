#include "mem_utils.h"

#include <stdlib.h>
#include <memory.h>

#define EXIT_FAILURE 1

void *cooler_realloc(void *source, size_t new_capacity) {
	void *res = realloc(source, new_capacity);
	if (NULL == res) {
		free(source);
		perror("realloc failed");
		exit(EXIT_FAILURE);
	}
	return res;
}

void *cooler_malloc(size_t size) {
	void *res = malloc(size);
	if (NULL == res) {
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}
	return res;
}

#
void *stralloc(size_t size) {
	char *res = cooler_malloc(size);
	res[size - 1] = '\0';
	return res;
}

#ifdef _WIN32
void *aligned_alloc(int allign, size_t size) {
	return cooler_malloc(size);
}
#endif