#pragma once

#include <stdio.h>

// Change these as you want
#define MAP_START_CAPACITY 16    // Should be pow 2
#define MAP_MAX_CAPACITY   65536 // Should be pow 2,
// never exceed int64 max
#define MAP_GROWTH_FACTOR  2
#define MAP_SHRINK_FACTOR  2

// Currently must be <=32, because of
// AVX2 registers
#define MAP_SMALL_STR_SIZE 32

// Error if <0 or >MAP_MAX_CAPACITY
typedef long long map_index_t;

struct map_data_entry {
    char *data;
    size_t len;
};
struct map_key {
    char *string;
    size_t len;
};

#ifdef _WIN32
#pragma pack(push, 1)
struct map {
#else
struct __attribute__((packed)) map {
#endif
    struct map_data_entry *data;
    struct map_key *keys;
    // A block of memory for short
    // strings (MAP_SMALL_STR_SIZE)
    // where they are stored contiguously
    char *short_key_store;
    map_index_t count;
    map_index_t capacity;
};
#ifdef _WIN32
#pragma push(pop)
#endif

void new_map(struct map *out_map);
void destroy_map(struct map *out_map);
int map_cpy_insert(
    struct map *out_map,
    const struct map_key *in_key,
    const char *restrict data,
    size_t data_size);
int map_mov_insert(
    struct map *out_map,
    const struct map_key *in_key,
    char *data,
    size_t data_size);
void map_erase(struct map *out_map, const struct map_key *in_key);
void map_erase_index(struct map *out_map, map_index_t index);
map_index_t map_get(
    const struct map *in_map,
    const struct map_key *in_key,
    struct map_data_entry **out_data);
