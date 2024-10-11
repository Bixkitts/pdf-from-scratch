#pragma once

#include "file_handling.h"
#include "markdown_utils.h"
#include "test_util.h"

test_result_t test_parse_markdown(void *)
{
    struct md_delimiter_results parse_results[MD_DELIM_COUNT] = {};
    struct mapped_file test_file                              = {};
    if (FILE_MAP_FAIL == memory_map_file("./test.md", &test_file)) {
        return TEST_FAIL;
    };
    parse_markdown(&test_file, parse_results);
    return TEST_SUCCEED;
}

void do_markdown_util_tests(struct test_results *out_results)
{
    run_test(
        test_parse_markdown,
        NO_TEST_PARAMS,
        "test_parse_markdown",
        out_results);
}
