#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "file_handling.h"

#include "converters.h"
#include "dict_util_tests.h"
#include "map_tests.h"
#include "pdf_tests.h"
#include "str_util_tests.h"

const char *run_tests_arg = "test";

enum operation {
    OP_DEFAULT,
    OP_DO_TESTS,
    OP_MD_TO_PDF
};

static void do_tests(void);
static enum operation parse_arguments(int argc, char *argv[]);
static int md_to_pdf(const char *file_in, const char *file_out);

int main(int argc, char *argv[])
{
    switch (parse_arguments(argc, argv)) {
    case OP_DO_TESTS:
        do_tests();
        break;
    case OP_MD_TO_PDF:
        if (md_to_pdf(argv[1], argv[2]) == -1) {
            return EXIT_FAILURE;
        };
        break;
    default:
        fprintf(stderr, "Usage: %s <markdown input> <pdf output>\n", argv[0]);
    };
    return EXIT_SUCCESS;
}

static int md_to_pdf(const char *file_in, const char *file_out)
{
    struct mapped_file test_md = {};
    if (memory_map_file(file_in, &test_md) == FILE_MAP_FAIL) {
        fprintf(stderr, "File mapping failed\n");
        return -1;
    }
    char *pdf      = NULL;
    size_t pdf_len = convert_md_to_pdf(test_md.data, test_md.size, &pdf);
    if (pdf_len == CONVERSION_FAILED) {
        fprintf(stderr, "MD to PDF conversion failed\n");
        return -1;
    }
    if (write_data_to_file(pdf, pdf_len, file_out) == FILE_WRITE_FAIL) {
        fprintf(stderr, "Writing output failed\n");
        return -1;
    }
    return 0;
}

static enum operation parse_arguments(int argc, char *argv[])
{
    if (argc == 2 && !strcmp((const char *)argv[1], run_tests_arg)) {
        return OP_DO_TESTS;
    }
    else if (argc == 3) {
        return OP_MD_TO_PDF;
    }
    return OP_DEFAULT;
}

static void do_tests(void)
{
    do_pdf_utils_tests();
    do_str_utils_tests();
    do_dict_util_tests();
    do_map_tests();

    char ch;
    printf("passed %d of %d.\n", p, t);
    printf("Press ENTER key to Continue\n");
    volatile int i = scanf("%c", &ch);
    i++;
}
