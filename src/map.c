#include <assert.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>

#ifdef __AVX2__
#include <immintrin.h>
#endif

#include <stddef.h>
#include <stdint.h>
#ifdef _WIN32
#include <stdbool.h>
#endif

#include "defines.h"
#include "map.h"
#include "mem_utils.h"

static map_index_t map_get_empty_index(const struct map *map)
{
    for (map_index_t i = 0; i < map->capacity; i++) {
        if (map->keys[i].string == NULL) {
            return i;
        }
    }
    return -1;
}

/* AVX 2 function to find 32 byte chunk    *
 * of data in an array of 32 byte chunks.  *
 * The array MUST be 32 byte aligned       *
 * for this to work at all,                *
 * and MUST have 32 byte elements.         *
 *                                         *
 * The chunk being searched for MUST       *
 * also be 32 byte aligned.                *
 *                                         *
 * TODO: This could be a helper function   *
 *       independant of the map            */
#ifdef __AVX2__
static int
find_32byte_chunk(const char *array, size_t array_len, const char *chunk)
{
    // TODO: not tested
    size_t array_size    = array_len * 32;
    __m256i chunk_vector = _mm256_load_si256((const __m256i *)chunk);

    for (size_t i = 0; i <= array_size; i += 32) {
        __m256i array_vector = _mm256_load_si256((const __m256i *)(array + i));

        // Compare the 32-byte chunks
        __m256i result = _mm256_cmpeq_epi8(array_vector, chunk_vector);

        // Check if all bytes in the result are 0xFF (meaning a full match)
        if (_mm256_movemask_epi8(result) == -1) {
            return (int)i; // Found a match, return the index
        }
    }

    return -1; // No match found
}
#endif

/* Returns the index in the map,     *
 * or -1 on failure to find the key. */
static map_index_t
map_find_key(const struct map *map, const struct map_key *restrict in_key)
{
// TODO: #ifdef in a function is not pretty
#ifdef __AVX2__
    if (in_key->len <= 32) {
        // Fast search over contiguous
        // map_key_store.
        // Currently AVX2 SIMD linear search,
        // swap it out with whatever.
        alignas(32) char tmp[32] = {};
        memcpy(tmp, in_key->string, in_key->len);
        return find_32byte_chunk(map->short_key_store, map->capacity, tmp);
    }
#endif
    // Slow search for long keys
    // (falls back to this)
    // Should still be very fast
    // because we skip unequal string lengths.
    for (map_index_t i = 0; i < map->capacity; i++) {
        const struct map_key *key = &map->keys[i];
        if (key->len != in_key->len) {
            continue;
        }
        if (key->string != NULL) {
            if (!strncmp(key->string, in_key->string, key->len)) {
                return i;
            };
        }
    }
    return -1;
}

static map_index_t
map_get_index_from_key(const struct map *in_map, const struct map_key *in_key)
{
    map_index_t slot = map_find_key(in_map, in_key);
    if (slot < 0) {
        slot = map_get_empty_index(in_map);
    }
    return slot;
}

static int map_resize(struct map *out_map, size_t capacity)
{
    if (out_map->capacity <= MAP_START_CAPACITY) {
        return 0;
    }
    if (capacity > MAP_MAX_CAPACITY) {
        return -1;
    }
    out_map->capacity = capacity;
    assert(sizeof(*out_map->data) == sizeof(*out_map->keys));
    const size_t data_size      = out_map->capacity * sizeof(*out_map->data);
    const size_t data_keys_size = data_size * 2;
    const size_t short_key_store_size = out_map->capacity * MAP_SMALL_STR_SIZE;
    const size_t current_size         = data_keys_size + short_key_store_size;
    const size_t total_alloc          = current_size;
    // Aligned for AVX2
    void *new_mem = aligned_alloc(32, total_alloc);
    if (!new_mem) {
        exit(1);
    }
    // We need to zero it because of
    // 256 bit string comparisons
    memset(new_mem, 0, total_alloc);
    memcpy(new_mem, out_map->data, data_size);
    memcpy((char *)new_mem + data_size, out_map->keys, data_size);
    memcpy((char *)new_mem + data_keys_size,
           out_map->short_key_store,
           data_size);
    free(out_map->data);
    out_map->data = new_mem;
    return 0;
}

/* THIS IS NOT A SEARCH FUNCTION.      *
 * We know if any string is in our     *
 * map short_key_store if it's memory  *
 * address falls within the block.     */
// static bool is_string_in_key_store(const struct map *map,
//                                    const char *string)
//{
//     return string < map->short_key_store
//            || string > map->short_key_store
//               + map->capacity * MAP_SMALL_STR_SIZE * sizeof(char);
// }

void new_map(struct map *out_map)
{
    // Allocate for the data and key structs
    // with one call
    assert(sizeof(*out_map->data) == sizeof(*out_map->keys));

    const size_t data_keys_size =
        MAP_START_CAPACITY * 2 * sizeof(*out_map->data);
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

    out_map->data = mem;
    out_map->keys = &((struct map_key *)mem)[MAP_START_CAPACITY];
    out_map->short_key_store =
        (char *)&((struct map_key *)mem)[MAP_START_CAPACITY * 2];
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
    for (int i = 0; i < out_map->capacity; i++) {
        // Strings smaller than 33 bytes
        // are copied into a contiguous key store
        // larger ones are allocated
        if (out_map->keys[i].len > MAP_SMALL_STR_SIZE) {
            free(out_map->keys[i].string);
        }
    }
    // We only have one allocation;
    // consult new_map()
    aligned_free(out_map->data);
    memset(out_map, 0, sizeof(*out_map));
}

/* Stores a key in the map at the given *
 * slot if it is empty, otherwise does  *
 * nothing. We should never write over  *
 * an occupied key in the map,          *
 * it needs to be explicitly erased     *
 * first                                */
static void map_try_store_key(
    struct map *out_map,
    map_index_t index,
    const struct map_key *in_key)
{
    if (out_map->keys[index].string) {
        return;
    }
    out_map->keys[index].len = in_key->len;
    if (in_key->len <= MAP_SMALL_STR_SIZE) {
        out_map->keys[index].string =
            &out_map->short_key_store[index * MAP_SMALL_STR_SIZE];
        memcpy(out_map->keys[index].string, in_key->string, in_key->len);
        return;
    }

    out_map->keys[index].string = calloc(in_key->len, sizeof(char));
    if (!out_map->keys[index].string) {
        exit(1);
    }
    memcpy(out_map->keys[index].string, in_key->string, in_key->len);
}

int map_cpy_insert(
    struct map *out_map,
    const struct map_key *in_key,
    const char *restrict data,
    size_t data_size)
{
    map_index_t index = map_get_index_from_key(out_map, in_key);
    if (index < 0) {
        if (map_resize(out_map, out_map->capacity * MAP_GROWTH_FACTOR)) {
            return -1;
        }
        map_cpy_insert(out_map, in_key, data, data_size);
    }
    out_map->count++;
    out_map->data[index].len = data_size;
    map_try_store_key(out_map, index, in_key);

    char **dest = &out_map->data[index].data;
    *dest       = cooler_malloc(data_size * sizeof(char));
    if (!*dest) {
        exit(1);
    }
    memcpy(*dest, data, data_size);
    return 0;
}

int map_mov_insert(
    struct map *out_map,
    const struct map_key *in_key,
    char *data,
    size_t data_size)
{
    map_index_t index = map_get_index_from_key(out_map, in_key);
    if (index < 0) {
        if (map_resize(out_map, out_map->capacity * MAP_GROWTH_FACTOR)) {
            return -1;
        }
        map_mov_insert(out_map, in_key, data, data_size);
    }
    map_try_store_key(out_map, index, in_key);
    out_map->count++;
    out_map->data[index].len  = data_size;
    out_map->data[index].data = data;
    return 0;
}

void map_erase_index(struct map *out_map, map_index_t index)
{
    free(out_map->data[index].data);
    memset(&out_map->data[index], 0, sizeof(*out_map->data));
    if (out_map->keys[index].len > MAP_SMALL_STR_SIZE) {
        free(out_map->keys[index].string);
    }
    else {
        memset(&out_map->short_key_store[index * MAP_SMALL_STR_SIZE],
               0,
               MAP_SMALL_STR_SIZE);
    }
    memset(&out_map->keys[index], 0, sizeof(*out_map->keys));
    out_map->count--;
    size_t shrink = out_map->capacity / MAP_SHRINK_FACTOR;
    if (out_map->count < ((map_index_t)shrink / 2)) {
        map_resize(out_map, shrink);
    }
}

void map_erase(struct map *out_map, const struct map_key *in_key)
{
    map_index_t index = map_find_key(out_map, in_key);
    if (index < 0) {
        return;
    }
    map_erase_index(out_map, index);
}

map_index_t map_get(
    const struct map *out_map,
    const struct map_key *in_key,
    struct map_data_entry **out_data)
{
    map_index_t index = map_find_key(out_map, in_key);
    if (index >= 0) {
        if (NULL != out_data) {
            *out_data = &out_map->data[index];
        }
        return index;
    }
    return -1;
}
