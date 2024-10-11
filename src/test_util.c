#include <assert.h>
#include <stdbool.h>

#ifdef ENABLE_TEST_RUNS
#include <stdio.h>
#endif

#if defined(SOFT_FAIL) || defined(ENABLE_TEST_RUNS)
#include "test_util.h"
#endif

#ifdef SOFT_FAIL
void iassert(int a)
{
    if (!a)
        printf("assertion failure...\n");
}
#endif

#ifdef ENABLE_TEST_RUNS
void run_test(
    test_result_t (*test)(void *),
    void *test_params,
    const char *test_name,
    struct test_results *out_results)
{
    if (!test_name) {
        test_name = "Unnamed Test";
    }
    printf("Running test \"%s\" : ", test_name);
    test_result_t test_res = test(test_params);
    if (test_res) {
        printf("SUCCEEDED\n");
    }
    else {
        printf("FAILED\n");
    }
    out_results->tests_passed += test_res;
    out_results->tests_done++;
}

void print_test_results(const struct test_results *results)
{
    printf("Passed %lld of %lld Tests.\n",
           results->tests_passed,
           results->tests_done);
}

void sum_test_results(
    const struct test_results *in_results,
    unsigned long long count,
    struct test_results *out_result)
{
    iassert(out_result->tests_done == 0 || out_result->tests_passed == 0);
    for (unsigned long i = 0; i < count; i++) {
        out_result->tests_done += in_results->tests_done;
        out_result->tests_passed += in_results->tests_passed;
    }
}
#endif
