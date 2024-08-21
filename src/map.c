#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include "defines.h"
#include <string.h>

#include "map.h"

static long long map_get_empty_slot(const struct map *map)
{
    for(long long i = 0; i < map->capacity; i++) {
        if(map->keys[i].string == NULL) {
            return i;
        }
    }
    return -1;
}

// TODO: optimise af.
// Make the strings contiguous and not alloc!!!!!
static long long map_find_key(const struct map *map,
                              const struct map_key *restrict in_key)
{
    for(long long i = 0; i < map->capacity; i++) {
        const struct map_key *key = &map->keys[i];
        if(key->len != in_key->len) {
            continue;
        }
        if(key->string != NULL) {
            if(!strncmp(key->string, in_key->string, key->len)) {
                return i;
            };
        }
    }
    return -1;
}

static long long map_get_slot_from_key(const struct map *in_map,
                                       const struct map_key *in_key)
{
    long long slot = map_find_key(in_map, in_key);
    if(slot < 0) {
        slot = map_get_empty_slot(in_map);
    }
    return slot;
}

static void map_grow(struct map *out_map)
{
    assert(sizeof(*out_map->data) == sizeof(*out_map->keys));
    const size_t data_keys_size       = out_map->capacity * 2
                                        * sizeof(*out_map->data);
    const size_t short_key_store_size = out_map->capacity * MAP_SMALL_STR_SIZE;
    const size_t current_size         = data_keys_size + short_key_store_size;
    const size_t total_alloc          = MAP_GROWTH_FACTOR * current_size;
    // Aligned for AVX2
    void *new_mem = aligned_alloc(32, total_alloc);
    if (!new_mem) {
        exit(1);
    }
    // We need to zero it because of
    // 256 bit string comparisons
    memset (new_mem, 0, total_alloc);
    memcpy (new_mem, out_map->data, current_size);
    free   (out_map->data);
    out_map->data = new_mem;
    out_map->capacity *= MAP_GROWTH_FACTOR;
}

/* We know if any string is in our     *
 * map short_key_store if it's memory  *
 * address falls within the block      */
static bool is_string_in_key_store(const struct map *map,
                                   const char *string)
{
    return string < map->short_key_store
           || string > map->short_key_store
              + map->capacity * MAP_SMALL_STR_SIZE * sizeof(char);
}

void new_map(struct map *out_map)
{
    // Allocate for the data and key structs
    // with one call
    assert(sizeof(*out_map->data) == sizeof(*out_map->keys));

    const size_t data_keys_size       = MAP_START_CAPACITY * 2
                                        * sizeof(*out_map->data);
    const size_t short_key_store_size = MAP_START_CAPACITY * MAP_SMALL_STR_SIZE;
    const size_t total_alloc          = data_keys_size + short_key_store_size;
    // Aligned for AVX2
    void *mem = aligned_alloc(32, total_alloc);
    if (!mem) {
        exit(1);
    }
    // We need to zero it because of
    // 256 bit string comparisons
    memset(mem, 0, total_alloc);

    out_map->data            = mem;
    out_map->keys            =        &((struct map_key*)mem)[MAP_START_CAPACITY];
    out_map->short_key_store = (char*)&((struct map_key*)mem)[MAP_START_CAPACITY*2];
    out_map->capacity        = MAP_START_CAPACITY;
    out_map->count           = 0;
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
    for (int i = 0; i < out_map->capacity; i++) {
        if(out_map->keys[i].string < out_map->short_key_store
           || out_map->keys[i].string > out_map->short_key_store
              + out_map->capacity * MAP_SMALL_STR_SIZE * sizeof(char))
        {
            // Strings smaller than 33 bytes
            // are copied into a contiguous key store
            // larger ones are allocated
            if(!is_string_in_key_store(out_map, out_map->keys[i].string)) {
                free(out_map->keys[i].string);
            }
        }
    }
    memset (out_map, 0, sizeof(*out_map));
    // Remember to free
}

static void map_store_key(struct map *out_map,
                          long long slot,                   
                          const struct map_key *in_key)
{
    if(in_key->len <= MAP_SMALL_STR_SIZE) {
        memcpy(out_map->keys[slot].string, in_key->string, in_key->len);
    }
    out_map->keys[slot].string = malloc(in_key->len);
    if(!out_map->keys[slot].string) {
        exit(1);
    }
    memcpy(out_map->keys[slot].string, in_key->string, in_key->len);

}

void map_cpy_insert(struct map *out_map,
                    const struct map_key *in_key,
                    const char *restrict data,
                    size_t data_size)
{
    long long slot = map_get_slot_from_key(out_map,
                                           in_key);
    if(slot < 0) {
        map_grow       (out_map);
        // Will always succeed on first recursion
        map_cpy_insert (out_map,
                        in_key,
                        data,
                        data_size);
    }
    out_map->count++;
    out_map->data[slot].len = data_size;
    // Copy in the string data from the search key
    // if we're inserting to an empty slot.
    // TODO: Not optimal, we do this IF() in the
    // map_get_slot_from_key() function already.
    if (!out_map->keys[slot].string) {

    }
    char *dest = out_map->data[slot].data;
    dest = malloc(data_size);
    if(!dest) {
        exit(1);
    }
    memcpy(dest, data, data_size);
}

void map_mov_insert(struct map *out_map,
                    const struct map_key *in_key,
                    char *data,
                    size_t data_size)
{
    long long slot = map_get_slot_from_key(out_map,
                                           in_key);
    if(slot < 0) {
        map_grow       (out_map);
        // Will always succeed on first recursion
        map_mov_insert (out_map,
                        in_key,
                        data,
                        data_size);
    }
    // Copy in the string data from the search key
    // if we're inserting to an empty slot.
    // TODO: Not optimal, we do this IF() in the
    // map_get_slot_from_key() function already.
    if (!out_map->keys[slot].string) {
        out_map->keys[slot].string = malloc(in_key->len);
        if(!out_map->keys[slot].string) {
            exit(1);
        }
        memcpy(out_map->keys[slot].string, in_key->string, in_key->len);
    }
    out_map->count++;
    out_map->data[slot].len  = data_size;
    out_map->data[slot].data = data;
}

void map_erase(struct map *map,
               const struct map_key *key)
{
}

struct map_data_entry *map_get(const struct map *map,
                               const struct map_key *key)
{
    return &map->data[map_find_key(map, key)];
}
