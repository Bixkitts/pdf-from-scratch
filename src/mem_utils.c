#include "mem_utils.h"

#include <stdlib.h>
#include <memory.h>

#define EXIT_FAILURE 1
#define SOFT

void *cooler_realloc(void *source, size_t new_capacity) {
	void *res = realloc(source, new_capacity);
#ifdef SOFT
	if (NULL == res) {
		free(source);
		perror("realloc failed");
		exit(EXIT_FAILURE);
	}
#endif
	return res;
}

void *cooler_malloc(size_t size) {
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