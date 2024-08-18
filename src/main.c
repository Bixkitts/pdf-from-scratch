#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "file_handling.h"
#include "pdf_utils.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *arg1 = argv[1];
    if (strcmp(arg1, "test") == 0) {
        do_tests();
    }
    else {
        mmap_and_parse_file(arg1);
    }
    return 0;
}

static int mmap_and_parse_file(const char* filename) {
    size_t file_size;
    char* file_content = memory_map_file(filename, &file_size);
    if (!file_content) {
        exit(EXIT_FAILURE);
    }

    size_t num_lines = 0;
    struct LineInfo* lines = parse_file(file_content, file_size, &num_lines);

    printf("Found %zu lines starting with '#':\n", num_lines);
    for (size_t i = 0; i < num_lines; i++) {
        printf("Line %zu: Start Index = %zu, Length = %zu\n",
            i, lines[i].line_index, lines[i].line_length);
    }

    free(lines);
    unmap_file(file_content, file_size);
}

static int do_tests() {
    do_pdf_utils_tests();
}