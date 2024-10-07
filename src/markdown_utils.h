#pragma once

#include <stdio.h>

#include "file_handling.h"

#define MD_DELIMITER_LEN 16

enum md_delimiter_type {
    MD_DELIM_TITLE,
    MD_DELIM_BULLET,
    MD_DELIM_COUNT
};

typedef size_t md_index_t;

struct md_delimiter {
    char open[MD_DELIMITER_LEN];
    char close[MD_DELIMITER_LEN];
};

// TODO: something like this?
struct md_delimiters {
    struct md_delimiter data[MD_DELIM_COUNT];
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
};

int parse_markdown(
    const struct md_delimiter *delims,
    int delim_count,
    const struct mapped_file *md,
    struct md_delimiter_results *out_results);
void delete_md_parse_results(
    struct md_delimiter_results *results,
    size_t count);
