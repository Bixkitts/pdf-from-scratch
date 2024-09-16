#include "benchmark.h"
#include "mem_utils.h"
#include "str_utils.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef _WIN32
#include <time.h>
#endif

/*
 * bench_clock_t needs to be
 * a printable double precision float value
 * indicating milliseconds on
 * any platform.
 */
#ifndef _WIN32
typedef double bench_clock_ms_t;
#else
// TODO: idk how this is on windows
typedef double bench_clock_ms_t;
#endif

// This is used to take a function's return
// value and do nothing with it without
// being optimised out
typedef volatile long long volatile_ret_t;

/*
 * Benchmark functions
 */
static int bench_strstr(void *input, size_t in_len);
static int bench_cooler_strstr(void *input, size_t in_len);

/*
 * Benchmark input generators.
 * If you make one, it NEEDS
 * to allocate *data.
 */
static size_t bench_gen_haystack_1(void **data, int iter);

static void run_benchmark(int (*bench_fn)(void *, size_t),
                          size_t (*input_gen)(void **, int),
                          int sample_count,
                          const char *name);
void do_all_benchmarks()
{
    const int str_search_sample_count = 100;
    srand(time(NULL));
    run_benchmark(bench_strstr,
                  bench_gen_haystack_1,
                  str_search_sample_count,
                  "strstr()");
    run_benchmark(bench_cooler_strstr,
                  bench_gen_haystack_1,
                  str_search_sample_count,
                  "cooler_strstr()");
}

static void run_benchmark(int (*bench_fn)(void *, size_t),
                          size_t (*input_gen)(void **, int),
                          int sample_count,
                          const char *name)
{
    volatile clock_t benchmark_total = 0;
    volatile clock_t benchmark_begin = 0;
    volatile clock_t benchmark_end   = 0;
    void *input                      = NULL;
    for (int i = 0; i < sample_count; i++) {
        size_t in_len   = input_gen(&input, i);
        benchmark_begin = clock();
        volatile_ret_t nop =
            bench_fn(input, in_len); // We purposefully do nothing with nop
        benchmark_end = clock();
        benchmark_total += benchmark_end - benchmark_begin;
    }
    const bench_clock_ms_t result =
        (bench_clock_ms_t)((benchmark_total * 1000.0f) / CLOCKS_PER_SEC);
    const bench_clock_ms_t result_avg = result / sample_count;
    printf("Benchmark %s %d samples: %fms\n", name, sample_count, result_avg);
    free(input);
}

static size_t bench_gen_haystack_1(void **data, int iter)
{
    const size_t test_str_len = 1000;
    iter                      = iter > 600 ? 600 : iter;
    if (!*data) {
        *data = cooler_malloc(test_str_len * sizeof(char));
    }
    char *haystack = *data;
    memset(haystack,
           (rand() % 90) + 33,
           test_str_len * sizeof(char)); // ASCII alphanumberic garbage
    haystack[test_str_len - 1] = '\0';
    const char *needle         = "needle";
    const int n_len            = strlen(needle);
    memcpy(&haystack[rand() % (test_str_len - n_len - 1)], needle, n_len);
    return test_str_len;
}

static int bench_strstr(void *input, size_t in_len)
{
    const char *needle = "needle";
    return (volatile_ret_t)strstr((char *)input, needle);
}

static int bench_cooler_strstr(void *input, size_t in_len)
{
    const char *needle = "needle";
    return (volatile_ret_t)cooler_strstr((char *)input, needle);
}
