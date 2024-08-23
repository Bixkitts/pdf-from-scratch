#include "mem_utils.h"
#include <stddef.h>
#include <stdlib.h>

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