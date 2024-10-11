#pragma once

#include "defines.h"
#include "mem_utils.h"
#include "str_utils.h"
#include "test_util.h"
#include <stdio.h>

test_result_t test_str_replace(void *)
{
    const char *test_str = "aaaatesaaaatest, test, est aaa";
    char *result         = str_replace(test_str, "test", "dupa");

    return strcmp(result, "aaaatesaaaadupa, dupa, est aaa") == 0;
}

test_result_t test_count_for_split_str_by_whitespace(void *)
{
    const char *test_str = "a    b c d   e\n f\t\n\v g";
    int res =
        count_for_split_str_by_whitespace(strdup(test_str), strlen(test_str));

    return res == 7;
}

test_result_t test_split_string_by_whitespace(void *)
{
    char *test_str = strdup("a    b c d   e\n f\t\n\v g");
    int cnt =
        count_for_split_str_by_whitespace(strdup(test_str), strlen(test_str));
    char **strs = cooler_malloc(sizeof(char *) * cnt);

    split_string_by_whitespace(test_str, strs, strlen(test_str), cnt);
    return strcmp(strs[6], "g") == 0;
}

test_result_t test_str_reverse(void *)
{
    char *test_str       = "hello world.";
    char *correct_result = ".dlrow olleh";
    str_rev(test_str);
    return !!strcmp(test_str, correct_result);
}

void do_str_utils_tests(struct test_results *out_results)
{
    run_test(
        test_count_for_split_str_by_whitespace,
        NO_TEST_PARAMS,
        "test_count_for_split_str_by_whitespace",
        out_results);
    run_test(
        test_split_string_by_whitespace,
        NO_TEST_PARAMS,
        "test_split_string_by_whitespace",
        out_results);
    run_test(test_str_replace, NO_TEST_PARAMS, "test_str_replace", out_results);
    // run_test(test_str_reverse, NO_TEST_PARAMS, "test_str_reverse",
    // out_results);
}
