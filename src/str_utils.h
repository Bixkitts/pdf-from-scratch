#pragma once

#include <stdio.h>

// Fixed str width we can do optimisations on
#define SHORT_STR_LEN 32

// TODO: optimise test and verify
void str_rev(char *str);

/* Alternate strstr, don't use until faster than strstr */
int cooler_strstr(const char *haystack, const char *needle);

#ifndef _WIN32
// TODO: optimise test and verify
char *itoa(int value, char *str, int base);
#endif

int ends_with(
    const char *str,
    const char *suffix,
    size_t str_len,
    size_t suffix_len);

int starts_with(
    const char *str,
    const char *prefix,
    size_t str_len,
    size_t prefix_len);

size_t
count_chars_before_string(const char *str, const char *before, size_t str_len);

size_t count_chars_after_string(
    const char *str,
    const char *after,
    size_t str_len,
    size_t after_len);

const char *find_nonspace(const char *begin, const char *end);

const char *find_space(const char *begin, const char *end);

#define INTERNAL_BSIZE 32
static char idc[INTERNAL_BSIZE];
char *lltoa_helper(long long val);

char *sizetoa_helper(size_t val);
#undef INTERNAL_BSIZE

long long strtoll_helper(char *str);

size_t strtoull_helper(char *str);

long long strtoll_helper_2(char *str, char *endstr);

size_t strtoull_helper_2(char *str, char *endstr);

int strarr_cmp(char **arra, char **arrb, size_t len);

int char_in_str(char ch, const char *ctrl);

#ifdef _WIN32
char *strsep(char **stringp, const char *delim);
#else
char *strpbrk(const char *str, const char *ctrl);
#endif

int count_for_split_str_by_whitespace(char *str, size_t str_len);

int split_string_by_whitespace(
    char *str,
    char **out,
    size_t str_len,
    int count);
char *str_replace(const char *orig, const char *rep, const char *with);
