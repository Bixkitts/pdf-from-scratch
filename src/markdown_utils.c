#define _GNU_SOURCE // For memmem()

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "markdown_utils.h"
#include "mem_utils.h"

#define MD_RESULT_ALLOC_GROW_FACTOR 2

struct md_delimiter {
    char open[MD_DELIMITER_LEN];
    char close[MD_DELIMITER_LEN];
};

const struct md_delimiter
default_md_delimiters[MD_DELIM_COUNT] =
{
    {"#", "\n"},
    {"-", "\n"}
};

struct md_index_result {
    md_index_t index;
    bool is_invalid;
};

static struct md_index_result find_next_delim(
    const char *delim,
    const struct mapped_file *md,
    size_t current_index);
static int md_results_increase_capacity(struct md_delimiter_results *out_res);

static struct md_index_result find_next_delim(
    const char *delim,
    const struct mapped_file *md,
    size_t current_index)
{
    struct md_index_result res = {0};
    char *current_loc          = (char *)md->data + current_index;
    while (current_index < md->size) {
        void *loc =
            memmem(current_loc, md->size - current_index, delim, strlen(delim));

        if (loc) {
            res.index = (char *)loc - (char *)md->data;
            break;
        }
        else {
            res.is_invalid = 1;
            break;
        }
    }
    return res;
}

static int md_results_increase_capacity(struct md_delimiter_results *out_res)
{
    size_t new_capacity = 0;
    if (out_res->capacity == 0) {
        new_capacity = 32;
    }
    else {
        new_capacity = out_res->capacity * MD_RESULT_ALLOC_GROW_FACTOR;
    }
    void *new_alloc = cooler_malloc(
        (new_capacity * sizeof(*out_res->indices)) +
        (new_capacity * sizeof(*out_res->lengths)));

    memcpy(new_alloc,
           out_res->indices,
           out_res->count * sizeof(*out_res->indices));
    out_res->indices = new_alloc;

    size_t *new_lengths_loc = (size_t *)out_res->indices + new_capacity;
    memcpy(new_lengths_loc,
           out_res->lengths,
           out_res->count * sizeof(*out_res->lengths));
    out_res->lengths = new_lengths_loc;

    out_res->capacity = new_capacity;

    free(out_res->allocation);
    out_res->allocation = new_alloc;
    return 0;
}

int parse_markdown(
    const struct mapped_file *md,
    struct md_all_delimiter_results *out_results)
{
    for (int i = 0; i < MD_DELIM_COUNT; i++) {
        md_results_increase_capacity(&out_results->array[i]);
        md_index_t cursor           = 0;
        unsigned int open_delim_len = strlen(default_md_delimiters[i].open);
        size_t result_count         = 0;

        while (cursor < md->size) {
            struct md_index_result open =
                find_next_delim(default_md_delimiters[i].open, md, cursor);
            if (open.is_invalid) {
                break;
            }
            open.index += open_delim_len;
            cursor = open.index;

            struct md_index_result close =
                find_next_delim(default_md_delimiters[i].close, md, cursor);
            if (close.is_invalid) {
                break;
            }
            cursor += close.index - open.index;

            out_results->array[i].indices[result_count] = open.index;
            out_results->array[i].lengths[result_count] =
                close.index - open.index;
            result_count++;
            out_results->array[i].count = result_count;

            if (result_count >= out_results->array[i].capacity) {
                md_results_increase_capacity(&out_results->array[i]);
            }
        }
    }
    return 0;
}

void delete_md_parse_results(struct md_all_delimiter_results *results)
{
    assert(results->array->allocation != NULL);
    free(results->array->allocation);
    results->array->allocation = NULL;
}

#undef _GNU_SOURCE
