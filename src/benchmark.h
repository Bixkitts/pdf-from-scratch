#pragma once

#include <stdio.h>

/*
 * To run benchmarks,
 * make functions with these
 * signatures and pass them
 * to run_benchmark().
 */

// Generate input for your benchmark.
// Return the length.
typedef size_t (*benchmark_input_gen)(void **input, int sample_iter);

// Pass the input and size to a benchmark function,
// always return 0.
typedef int (*benchmark_fn)(void *input, size_t input_len);

// This will be called after all benchmark
// samples to clean up allocated memory
// or open file handles etc.
typedef void (*benchmark_input_cleanup)(void **input, size_t input_len);

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
void print_benchmark(bench_clock_ms_t result,
                     int sample_count,
                     const char *name);
bench_clock_ms_t run_benchmark(benchmark_fn bench_fn,
                               benchmark_input_gen input_gen,
                               benchmark_input_cleanup input_clean,
                               int sample_count);
