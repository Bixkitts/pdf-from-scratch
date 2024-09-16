/*
 * Functions
 * for benchmarking.
 * This is included
 * exactly once in main.c
 * AND NOWHERE ELSE
 */

#include "mem_utils.h"
#include "str_utils.h"
#include <memory.h>
#include <stdlib.h>

size_t bench_gen_haystack_1(void **data, int iter)
{
    const size_t test_str_len = 1000;
    iter                      = iter > 600 ? 600 : iter;
    if (!*data) {
        *data = cooler_malloc(test_str_len * sizeof(char));
    }
    char *haystack = *data;
    for (size_t i = 0; i < test_str_len; i++) {
        haystack[i] = (rand() % 90) + 33;
    }
    haystack[test_str_len - 1] = '\0';
    const char *needle         = "needle";
    const int n_len            = strlen(needle);
    memcpy(&haystack[rand() % (test_str_len - n_len - 1)], needle, n_len);
    return test_str_len;
}

void bench_haystack_1_cleanup(void **input, size_t input_len)
{
    free(*input);
}

int bench_strstr(void *input, size_t in_len)
{
    const char *needle = "needle";
    return (long long)strstr((char *)input, needle);
    return 0;
}

int bench_cooler_strstr(void *input, size_t in_len)
{
    const char *needle = "needle";
    cooler_strstr((char *)input, needle);
    return 0;
}
