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

void run_benchmark(benchmark_fn bench_fn,
                   benchmark_input_gen input_gen,
                   benchmark_input_cleanup input_clean,
                   int sample_count,
                   const char *name)
{
    volatile clock_t benchmark_total = 0;
    volatile clock_t benchmark_begin = 0;
    volatile clock_t benchmark_end   = 0;
    void *input                      = NULL;
    size_t in_len                    = 0;
    for (int i = 0; i < sample_count; i++) {
        in_len          = input_gen(&input, i);
        benchmark_begin = clock();
        volatile int nop =
            bench_fn(input, in_len); // We purposefully do nothing with nop
        benchmark_end = clock();
        benchmark_total += benchmark_end - benchmark_begin;
    }
    const bench_clock_ms_t result =
        (bench_clock_ms_t)((benchmark_total * 1000.0f) / CLOCKS_PER_SEC);
    const bench_clock_ms_t result_avg = result / sample_count;
    printf("Benchmark %s %d samples: %fms\n", name, sample_count, result_avg);
    input_clean(&input, in_len);
}
