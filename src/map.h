#pragma once

#include <stdio.h>

// Change these as you want
#define MAP_START_CAPACITY 16    // Should be pow 2
#define MAP_MAX_CAPACITY   65536 // Should be pow 2,
                                 // never exceed int64 max
#define MAP_GROWTH_FACTOR  2
#define MAP_SHRINK_FACTOR  2

// MUST be 32, always, because of
// AVX 256
#define MAP_SMALL_STR_SIZE 32

struct map_data_entry {
    char   *data;
    size_t  len;
};
struct map_key {
    char   *string;
    size_t  len;
};
// TODO:
// Could mb do smthn like this,
// because the map member
// "data" points to allocated
// memory pool for the whole
// struct:
//
// union map_data {
//     struct map_data_entry *data;
//     void *memory;
// };
//
#ifdef _WIN32
struct map {
#else
struct __attribute__((packed)) map {
#endif
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
 * inserts.                                 *
 * Returns -1 on failure (map full)         */
int   map_cpy_insert  (struct map *out_map,
                       const struct map_key *in_key,
                       const char *restrict data,
                       size_t data_size);

/* This function takes ownership of         *
 * the data it's passed.                    *
 * Returns -1 on failure (map full)         */
int   map_mov_insert  (struct map *out_map,
                       const struct map_key *in_key,
                       char *data,
                       size_t data_size);

/* Remember to get and destroy objects      *
 * before erasing them.                     */
void  map_erase       (struct map *out_map,
                       const struct map_key *in_key);
/* Index is NOT checked,                *
 * will crash the program if invalid.   *
 * Use index returned from map_get.     */
void  map_erase_index (struct map *out_map,
                       long long index);

/* Returns the index to the object in the   *
 * map, and sets out_data to point to it.   *
 * If the key is not found, the function    *
 * returns -1 and doesn't modify out_data.  *
 * Pass the returned index to               *
 * map_erase_index().                       */
long long map_get (const struct map *in_map,
                   const struct map_key *in_key,
                   struct map_data_entry *out_data);
