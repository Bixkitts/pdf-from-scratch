#pragma once

#include <string.h>

#include "file_handling.h"
#include "markdown_utils.h"
#include "test_util.h"

test_result_t test_parse_markdown(void *)
{
    struct md_all_delimiter_results parse_results = {};
    char *test_markdown = "some text\n#This is a title.\nThis is text...#And "
                          "This Is Another Title\n - bullet point\nfinal text";
    struct mapped_file test_file =
        {.data = test_markdown, .size = strlen(test_markdown)};
    parse_markdown(&test_file, &parse_results);
    if (parse_results.array[MD_DELIM_TITLE].count == 2 &&
        parse_results.array[MD_DELIM_TITLE].indices[0] == 11 &&
        parse_results.array[MD_DELIM_TITLE].lengths[0] == 16 &&
        parse_results.array[MD_DELIM_TITLE].lengths[1] == 25 &&
        parse_results.array[MD_DELIM_BULLET].lengths[0] == 13) {
        return TEST_SUCCEED;
    }
    return TEST_FAIL;
}

void do_markdown_util_tests(struct test_results *out_results)
{
    run_test(
        test_parse_markdown,
        NO_TEST_PARAMS,
        "test_parse_markdown",
        out_results);
}
