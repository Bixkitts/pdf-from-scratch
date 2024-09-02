#pragma once

#include <string.h>

#include "defines.h"
#include "test_util.h"
#include "map.h"

#define MAP_TEST_BIG_LEN   64
#define MAP_TEST_SMALL_LEN 16

int test_map_big_insert(void)
{
    struct map test_map = {}; 
    const int  rubbish  = 111;
    new_map(&test_map);

    char data[MAP_TEST_BIG_LEN] = "";
    memset(data, rubbish, MAP_TEST_BIG_LEN);

    char key_str[MAP_TEST_BIG_LEN] = "test_key";
    const struct map_key key = {.string = key_str,
                                .len    = MAP_TEST_BIG_LEN};

    map_cpy_insert(&test_map,
                   &key,
                   data,
                   MAP_TEST_BIG_LEN);
#ifndef _WIN32
    struct map_data_entry *comp = NULL;
    map_get(&test_map, &key, &comp);
    int res = (!strncmp(comp->data,
                        data,
                        MAP_TEST_BIG_LEN));
    destroy_map(&test_map);
    return res;
#else
    return 0;
#endif
}

int test_map_small_insert(void)
{
    struct map test_map = {}; 
    const int  rubbish  = 111;
    new_map(&test_map);

    char data[MAP_TEST_SMALL_LEN] = "";
    memset(data, rubbish, MAP_TEST_SMALL_LEN);

    char key_str[MAP_TEST_SMALL_LEN] = "test_key";
    const struct map_key key = {.string = key_str,
                                .len    = strlen(key_str)};

    map_cpy_insert(&test_map,
                   &key,
                   data,
                   MAP_TEST_SMALL_LEN);

#ifndef _WIN32
    struct map_data_entry *comp = NULL;
    map_get(&test_map, &key, &comp);
    int res = (!strncmp(comp->data,
                        data,
                        MAP_TEST_SMALL_LEN));
    destroy_map(&test_map);
    return res;
#else
    return 0;
#endif
}

void do_map_tests(void)
{
    RUNTEST(test_map_big_insert);
    RUNTEST(test_map_small_insert);
}
