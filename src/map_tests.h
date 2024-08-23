#pragma once

#include <string.h>
#include "defines.h"
#include "test_util.h"
#include "map.h"

#define MAP_TEST_BIG_LEN 64

int test_map_big_insert(void)
{
    struct map test_map = {}; 
    new_map(&test_map);
    char data   [MAP_TEST_BIG_LEN] = "test_data";
    char key_str[MAP_TEST_BIG_LEN] = "test_key";
    const struct map_key key = {.string = key_str,
                                .len    = strlen(key_str)};
    map_cpy_insert(&test_map,
                   &key,
                   data,
                   MAP_TEST_BIG_LEN);
    return(!strncmp(map_get(&test_map, &key)->data,
                    data,
                    64));
}

void do_map_tests(void)
{
    RUNTEST(test_map_big_insert);
}
