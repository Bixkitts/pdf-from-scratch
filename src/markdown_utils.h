#pragma once

#include <stdio.h>

#define MD_DELIMITER_LEN 16

typedef size_t md_index_t;

struct md_delimiter {
    char open[MD_DELIMITER_LEN];
    char close[MD_DELIMITER_LEN];
};

struct md_parse_result {
    md_index_t *indices;
    size_t *lengths;
};

int parse_markdown(const struct md_delimiter *delims,
                   int delim_count,
                   struct md_parse_result *out_result);
void delete_md_parse_result(struct md_parse_result *result);
