#pragma once

// This doesn't belong in here??
#define STARRLEN(starr) (sizeof(starr) / sizeof(starr[0]))

#define TEST_FAIL       0
#define TEST_SUCCEED    1
#define NO_TEST_PARAMS  NULL
#define NO_TEST_NAME    NULL

typedef bool test_result_t;

struct test_results {
    unsigned long long tests_done;
    unsigned long long tests_passed;
};

#ifdef SOFT_FAIL
void iassert(int a);
#else
#define iassert(a) assert(a)
#endif

#ifdef ENABLE_TEST_RUNS
/* Not thread safe */
void run_test(
    test_result_t (*test)(void *),
    void *test_params,
    const char *test_name,
    struct test_results *out_results);
void print_test_results(const struct test_results *results);
/* You can run series of tests on different
 * threads, and then aggregate the results after
 * they are joined */
void sum_test_results(
    const struct test_results *in_results,
    unsigned long long count,
    struct test_results *out_result);
#else
#define run_test(a, b, c, h)  ((void)0)
#define print_test_results(a) ((void)0)
#define sum_test_results(a, b, c)  ((void)0)
#endif
