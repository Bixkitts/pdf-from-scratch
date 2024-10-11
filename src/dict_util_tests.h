#pragma once

#include <string.h>

#include "defines.h"
#include "dict_util.h"
#include "test_util.h"

test_result_t test_str_dict_to_arr(void *)
{
    const char *ex_dict2 =
        "<<"
        "/Item1 0.4"
        "/Item2 true"
        "/LastItem (not !)"
        "/VeryLastItem (OK)"
        ">>";

    char **out = NULL;
    int items  = str_dict_to_arr(strdup(ex_dict2), out);
    if (!out || items < 6) {
        return TEST_FAIL;
    }
    for (int i = 0; i < 6; i++) {
        if (!out[i]) {
            return TEST_FAIL;
        }
    }
    if (strcmp(out[0], "<<")) {
        return TEST_FAIL;
    }
    if (strcmp(out[1], "/Item1 0.4")) {
        return TEST_FAIL;
    }
    if (strcmp(out[2], "/Item2 true")) {
        return TEST_FAIL;
    }
    if (strcmp(out[3], "/LastItem (not !)")) {
        return TEST_FAIL;
    }
    if (strcmp(out[4], "/VeryLastItem (OK)")) {
        return TEST_FAIL;
    }
    if (strcmp(out[5], ">>")) {
        return TEST_FAIL;
    };
    return TEST_SUCCEED;
}

test_result_t test_find_nested_in_dict_str(void *)
{
    const char *out_dict =
        "<</Type / Example\n"
        "/Subtype /DictionaryExample\n"
        "/Version <<>>\n"
        "/IntegerItem 12\n"
        "/StringItem (a string)\n"
        "/Subdictionary "
        "{{{tagXX}}} r\n"
        ">>\n"
        "{{{tagXX}}} obj\n"
        "<<\n"
        "/Item1 0.4\n"
        "/Item2 true\n"
        "/LastItem (not !)\n"
        "/VeryLastItem (OK)"
        ">>>>>>>>>>>>>>>>>>>";
    if (!find_nested_in_dict_str(out_dict, strlen(out_dict))) {
        return TEST_FAIL;
    }
    return TEST_SUCCEED;
}

test_result_t test_overflow_dict(void *)
{
    const char *out_dict = "<<";
    if (!find_nested_in_dict_str(out_dict, strlen(out_dict))) {
        return TEST_FAIL;
    };
    return TEST_SUCCEED;
}

void do_dict_util_tests(struct test_results *out_results)
{
    run_test(
        test_str_dict_to_arr,
        NO_TEST_PARAMS,
        "str_dict_to_arr",
        out_results);
    run_test(
        test_find_nested_in_dict_str,
        NO_TEST_PARAMS,
        "find_nested_in_dict_str",
        out_results);
    run_test(test_overflow_dict, NO_TEST_PARAMS, "overflow_dict", out_results);
}
