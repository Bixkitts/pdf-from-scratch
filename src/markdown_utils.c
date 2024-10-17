#define _GNU_SOURCE // For memmem()

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "markdown_utils.h"
#include "mem_utils.h"

#define INDEX_RESULT_ALLOC_GROW_FACTOR 2

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

static struct md_index_result find_next_delim_open(
    const struct md_delimiter *delim,
    const struct mapped_file *md,
    size_t current_index);
static struct md_index_result find_next_delim_close(
    const struct md_delimiter *delim,
    const struct mapped_file *md,
    size_t current_index);
static int realloc_results(struct md_delimiter_results *out_res, size_t count);

static struct md_index_result find_next_delim_open(
    const struct md_delimiter *delim,
    const struct mapped_file *md,
    size_t current_index)
{
    struct md_index_result res = {0};
    char *current_loc          = (char *)md->data + current_index;
    while (current_index < md->size) {
        void *loc = memmem(
            current_loc,
            md->size - current_index,
            delim->open,
            strlen(delim->open));

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

static struct md_index_result find_next_delim_close(
    const struct md_delimiter *delim,
    const struct mapped_file *md,
    size_t current_index)
{
    struct md_index_result res = {0};
    char *current_loc          = (char *)md->data + current_index;
    while (current_index < md->size) {
        void *loc = memmem(
            current_loc,
            md->size - current_index,
            delim->close,
            strlen(delim->close));

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

static int realloc_results(struct md_delimiter_results *out_res, size_t new_capacity)
{
    assert(new_capacity > out_res->capacity);
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
    const md_index_t base_result_alloc_size = 32;
    md_index_t result_alloc_size            = base_result_alloc_size;
    for (int i = 0; i < MD_DELIM_COUNT; i++) {
        realloc_results(&out_results->array[i], base_result_alloc_size);
        md_index_t cursor           = 0;
        unsigned int open_delim_len = strlen(default_md_delimiters[i].open);
        size_t result_count         = 0;

        while (cursor < md->size) {
            struct md_index_result open =
                find_next_delim_open(&default_md_delimiters[i], md, cursor);
            if (open.is_invalid) {
                break;
            }
            open.index += open_delim_len;
            cursor = open.index;

            struct md_index_result close =
                find_next_delim_close(&default_md_delimiters[i], md, cursor);
            if (close.is_invalid) {
                break;
            }
            cursor += close.index - open.index;

            out_results->array[i].indices[result_count] = open.index;
            out_results->array[i].lengths[result_count] =
                close.index - open.index;
            result_count++;
            out_results->array[i].count = result_count;

            if (result_count >= result_alloc_size) {
                result_alloc_size *= INDEX_RESULT_ALLOC_GROW_FACTOR;
                realloc_results(&out_results->array[i], result_alloc_size);
            }
        }
    }
    return 0;
}

void delete_md_parse_results(
    struct md_all_delimiter_results *results,
    size_t count)
{
    for (size_t i = 0; i < count; i++) {
        assert(results->array[i].allocation != NULL);
        free(results->array[i].allocation);
    }
}

#undef _GNU_SOURCE
