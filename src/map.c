#include <stdlib.h>
#include <memory.h>
#include <assert.h>

#include "map.h"

static size_t map_get_empty_slot(const struct map *map)
{

}

void new_map(struct map *out_map)
{
    // Allocate for the data and key structs
    // with one call
    assert(sizeof(*out_map->data) == sizeof(*out_map->keys));

    const int key_data_mem  = MAP_START_CAPACITY * 2 * sizeof(*out_map->data);
    const int occupancy_mem = (MAP_START_CAPACITY * 2) / 8; // bit field
    const int total_alloc   = key_data_mem + occupancy_mem;
    void *mem = calloc(total_alloc, 1);
    if (!mem) {
        exit(1);
    }

    out_map->data     = mem;
    out_map->keys     = &((struct map_key*)mem)[MAP_START_CAPACITY];
    out_map->capacity = MAP_START_CAPACITY;
    out_map->count    = 0;
}

void destroy_map(struct map *out_map)
{
    // NULL checks are for cowards
    // and I'm a coward
    if (!out_map->data) {
        return;
    }
    // We only have one allocation;
    // consult new_map()
    free   (out_map->data);
    memset (out_map, 0, sizeof(*out_map));
}

void  map_cpy_insert(struct map *out_map,
                     const char *restrict key,
                     const char *restrict data,
                     size_t data_size)
{
}

void  map_mov_insert(struct map *out_map,
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
