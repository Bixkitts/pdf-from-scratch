#include <stdio.h>

struct map_data_entry {
    char   *data;
    size_t  len;
};
struct map_key {
    char   *string;
    size_t  len;
};
struct map {
    struct map_data_entry *data;
    struct map_key        *keys;
    size_t                 count;
    // Map inserting and erasing will
    // increase or decrease the capacity
    // and reallocate accordingly
    size_t                 capacity;
};


// crashes on failure to allocate
void new_map     (struct map *out_dict);

void destroy_map (struct map *out_dict);

/* This function copies data it             *
 * inserts.                                 */
void  map_cpy_insert  (struct map *map,
                       const char *key,
                       const char *data,
                       size_t data_size);

/* This function takes ownership of         *
 * the data it's passed.                    */
void  map_mov_insert  (struct map *map,
                       const char *key,
                       char *data);

/* Remember to get and destroy objects      *
 * before erasing them.                     */
void  map_erase       (struct map *map,
                       const char *key);

/* Returns NULL if an object corresponding  *
 * to a key was not found, otherwise        *
 * returns the object.                      */
void *map_get         (const struct map *map,
                       const char *key);
