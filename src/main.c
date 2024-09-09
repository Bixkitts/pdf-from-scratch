#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "file_handling.h"

#include "converters.h"
#include "dict_util_tests.h"
#include "map_tests.h"
#include "pdf_tests.h"
#include "str_util_tests.h"

static void do_tests(void);

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *arg1 = argv[1];
    if (strcmp(arg1, "test") == 0) {
        do_tests();
        return EXIT_SUCCESS;
    }
    struct mapped_file test_md = {};
    if (memory_map_file("./test.md", &test_md) == FILE_MAP_FAIL) {
        fprintf(stderr, "File mapping failed\n");
        return EXIT_FAILURE;
    }
    char *pdf      = NULL;
    size_t pdf_len = convert_md_to_pdf(test_md.data, test_md.size, &pdf);
    if (pdf_len == CONVERSION_FAILED) {
        fprintf(stderr, "MD to PDF conversion failed\n");
        return EXIT_FAILURE;
    }
    if (write_data_to_file(pdf, pdf_len, "./test.pdf") == FILE_WRITE_FAIL) {
        fprintf(stderr, "Writing output failed\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
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
