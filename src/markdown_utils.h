#pragma once

#include <stdio.h>

#include "file_handling.h"

#define MD_DELIMITER_LEN 16

typedef size_t md_index_t;

enum md_delimiter_type {
    MD_DELIM_TITLE,
    MD_DELIM_BULLET,
    MD_DELIM_COUNT
};

// Where the delimiters are found
// and how long their contents are
struct md_delimiter_results {
    void *allocation;
    // indices and lengths point into
    // the allocation
    md_index_t *indices;
    size_t *lengths;
    size_t count;
    size_t capacity;
};

struct md_all_delimiter_results {
    struct md_delimiter_results array[MD_DELIM_COUNT];
};

int parse_markdown(
    const struct mapped_file *md,
    struct md_all_delimiter_results *out_results);
void delete_md_parse_results(struct md_all_delimiter_results *results);
