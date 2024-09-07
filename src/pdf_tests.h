#pragma once

#include "mem_utils.h"
#include "pdf_utils.h"
#include "test_util.h"

int obj_length_test(void)
{
    const char *ex_obj[] = {"4 0 obj\n"
                            "<<"
                            "/Length 9",
                            "\n>>\n"
                            "stream\n",
                            "AAAAAAAAA",
                            "\nendstream\n"};

    return get_obj_length2(ex_obj, STARRLEN(ex_obj)) == 50;
}

int obj_replace_test(void)
{
    char *ex_obj[] = {"4 0 obj\n"
                      "<<"
                      "/Length {{{leee n}}}",
                      "\n>>\n"
                      "stream\n",
                      "{{{leee n}}}",
                      "\nendstream\n"};

    return replace_label_in_obj(ex_obj, STARRLEN(ex_obj), "leee n", "test") > 1;
}

int obj_stream_len_is_9(void)
{
    const char *ex_obj[] = {"4 0 obj\n"
                            "<<"
                            "/Length {{{leee n}}}",
                            "\n>>\n"
                            "stream\n",
                            "AAAAAAAAA",
                            "\nendstream\n"};
    size_t slen          = get_stream_length(ex_obj, STARRLEN(ex_obj));
    return slen == 9;
}

int obj_stream_pastes_correctly(void)
{
    char *ex_obj[] = {"4 0 obj\n"
                      "<<"
                      "/Length {{{leee n}}}",
                      "\n>>\n"
                      "stream\n",
                      "AAAAAAAAA",
                      "\nendstream\n"};
    size_t slen    = get_stream_length((const char **)ex_obj, STARRLEN(ex_obj));
    char *lenstr   = sizetoa_helper(slen);
    replace_label_in_obj(ex_obj, STARRLEN(ex_obj), "leee n", lenstr);
    return strcmp(ex_obj[0],
                  "4 0 obj\n"
                  "<<"
                  "/Length 9") == 0;
}

int obj_lens_calculated_properly(void)
{
    const char *ex_arr[] = {"a", "b", "c"};
    size_t ex_arr_len    = STARRLEN(ex_arr);
    size_t *lens         = cooler_malloc(sizeof(size_t) * ex_arr_len);
    get_obj_length(ex_arr, STARRLEN(ex_arr), lens);

    return lens[0] == 1 && lens[1] == 1 && lens[2] == 1;
}

int obj_lens_real_example_calculated_properly(void)
{
    const char *ex_obj[] = {"4 0 obj\n"
                            "<<"
                            "/Length {{{leee n}}}",
                            "\n>>\n"
                            "stream\n",
                            "AAAAAAAAA",
                            "\nendstream\n"};
    size_t ex_obj_len    = STARRLEN(ex_obj);
    size_t *lens         = cooler_malloc(sizeof(size_t) * ex_obj_len);
    get_obj_length(ex_obj, STARRLEN(ex_obj), lens);

    return lens[3] == 11;
}

int obj_join_works(void)
{
    char *ex_obj[] = {"4 0 obj\n"
                      "<<"
                      "/Length 9",
                      "\n>>\n"
                      "stream\n",
                      "AAAAAAAAA",
                      "\nendstream\n"};
    char *out;
    obj_join(&out, (const char **)ex_obj, STARRLEN(ex_obj));
    return strcmp(out,
                  "4 0 obj\n"
                  "<<"
                  "/Length 9"
                  "\n>>\n"
                  "stream\n"
                  "AAAAAAAAA"
                  "\nendstream\n") == 0;
}

void do_pdf_utils_tests(void)
{
    RUNTEST(obj_replace_test);
    RUNTEST(obj_stream_len_is_9);
    RUNTEST(obj_stream_pastes_correctly);
    RUNTEST(obj_length_test);
    RUNTEST(obj_lens_calculated_properly);
    RUNTEST(obj_lens_real_example_calculated_properly);
    RUNTEST(obj_join_works);
}
