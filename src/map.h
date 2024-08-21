#pragma once

#include <stdio.h>

#define MAP_START_CAPACITY 16
#define MAP_GROWTH_FACTOR  2
#define MAP_SHRINK_FACTOR  2
#define MAP_SMALL_STR_SIZE 32

struct map_data_entry {
    char   *data;
    size_t  len;
};
struct map_key {
    char   *string;
    size_t  len;
};

struct __attribute__((packed)) map {
    struct map_data_entry *data;
    struct map_key        *keys;
    // A block of memory for short
    // strings (MAP_SMALL_STR_SIZE)
    // so they are contiguous
    // and can be eaten by search
    // algos
    char                  *short_key_store;
    long long              count;
    // Map inserting and erasing will
    // increase or decrease the capacity
    // and reallocate accordingly
    long long              capacity;
};

// crashes on failure to allocate
void new_map     (struct map *out_map);

void destroy_map (struct map *out_map);

/* This function copies data it             *
 * inserts.                                 */
void  map_cpy_insert  (struct map *out_map,
                       const struct map_key *in_key,
                       const char *restrict data,
                       size_t data_size);

/* This function takes ownership of         *
 * the data it's passed.                    */
void  map_mov_insert  (struct map *out_map,
                       const struct map_key *in_key,
                       char *data,
                       size_t data_size);

/* Remember to get and destroy objects      *
 * before erasing them.                     */
void  map_erase       (struct map *out_map,
                       const struct map_key *in_key);

/* Returns NULL if an object corresponding  *
 * to a key was not found, otherwise        *
 * returns the object.                      */
struct map_data_entry *map_get (const struct map *out_map,
                                const struct map_key *in_key);
