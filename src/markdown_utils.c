#define _GNU_SOURCE // For memmem()

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "markdown_utils.h"
#include "mem_utils.h"

struct md_index_result {
    md_index_t index;
    bool isInvalid;
};

static struct md_index_result find_next_delim_open(
    const struct md_delimiter *delim,
    const struct mapped_file *md,
    size_t current_index)
{
    struct md_index_result res = {0};
    while (current_index < md->size) {
        void *loc =
            memmem(md->data, md->size, delim->open, strlen(delim->open));

        if (loc) {
            res.index = (char *)md->data - (char *)loc;
            break;
        }
        else {
            res.isInvalid = 1;
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
    while (current_index < md->size) {
        void *loc =
            memmem(md->data, md->size, delim->open, strlen(delim->open));

        if (loc) {
            res.index = (char *)md->data - (char *)loc;
            break;
        }
        else {
            res.isInvalid = 1;
            break;
        }
    }
    return res;
}

static int realloc_results(struct md_delimiter_results *out_res, size_t count)
{
    assert(count > out_res->count);
    void *new_alloc = cooler_malloc((count * sizeof(*out_res->indices)) +
                                    (count * sizeof(*out_res->lengths)));

    memcpy(new_alloc,
           out_res->indices,
           out_res->count * sizeof(*out_res->indices));
    out_res->indices = new_alloc;

    size_t *new_lengths_loc = (size_t *)out_res->indices + count;
    memcpy(new_lengths_loc,
           out_res->lengths,
           out_res->count * sizeof(*out_res->lengths));
    out_res->lengths = new_lengths_loc;

    out_res->count = count;

    free(out_res->allocation);
    out_res->allocation = new_alloc;
    return 0;
}

int parse_markdown(const struct md_delimiter *delims,
                   int delim_count,
                   const struct mapped_file *md,
                   struct md_delimiter_results *out_results)
{
    const md_index_t base_result_alloc_size = 32;
    md_index_t result_alloc_size            = base_result_alloc_size;
    for (int i = 0; i < delim_count; i++) {
        realloc_results(&out_results[i], base_result_alloc_size);
        md_index_t cursor   = 0;
        size_t result_count = 0;
        while (cursor < md->size) {
            struct md_index_result open =
                find_next_delim_open(&delims[i], md, cursor);
            if (open.isInvalid) {
                break;
            }
            cursor = open.index;
            struct md_index_result close =
                find_next_delim_close(&delims[i], md, cursor);
            if (close.isInvalid) {
                break;
            }
            cursor                               = close.index;
            out_results[i].indices[result_count] = open.index;
            out_results[i].lengths[result_count] = close.index - open.index;
            out_results[i].count                 = result_count;
            result_count++;
            if (result_count > result_alloc_size) {
                result_alloc_size *= 2;
                realloc_results(&out_results[i], result_alloc_size);
            }
        }
    }
    return 0;
}

void delete_md_parse_results(struct md_delimiter_results *results, size_t count)
{
    for (size_t i = 0; i < count; i++) {
        assert(results[i].allocation != NULL);
        free(results[i].allocation);
    }
}

#undef _GNU_SOURCE
