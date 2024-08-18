#include <stdlib.h>
#include <assert.h>

#include "map.h"

void new_map(struct map *out_dict)
{
    const size_t start_capacity = 16;
    out_dict->capacity = start_capacity;
    out_dict->count    = 0;
    // Allocate for the data and key structs
    // with one call
    assert(sizeof(*out_dict->data) == sizeof(*out_dict->keys));
    void *mem = calloc(start_capacity * 2, sizeof(*out_dict->data));
    if (!mem) {
        exit(1);
    }

    out_dict->data = mem;
    out_dict->keys = &((struct map_key*)mem)[start_capacity];
}

void destroy_map(struct map *out_dict)
{
}

void  map_cpy_insert(struct map *map,
                     const char *restrict key,
                     const char *restrict data,
                     size_t data_size)
{
}
void  map_mov_insert(struct map *map,
                     const char *restrict key,
                     const char *restrict data)
{
}

void  map_erase(struct map *map,
                const char *key)
{
}

void *map_get(const struct map *map,
              const char *key)
{
}
