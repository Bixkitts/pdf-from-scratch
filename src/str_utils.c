#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "mem_utils.h"
#include "str_utils.h"

// TODO: optimise test and verify
void str_rev(char *str)
{
    size_t length = strlen(str);
    for (size_t i = 0; i < length / 2; i++) {
        char temp           = str[i];
        str[i]              = str[length - i - 1];
        str[length - i - 1] = temp;
    }
}

static void str_search_compute_lps(const char *needle, int m, int *lps)
{
    int len = 0;
    lps[0]  = 0;
    int i   = 1;
    while (i < m) {
        if (needle[i] == needle[len]) {
            len++;
            lps[i] = len;
            i++;
        }
        else {
            if (len != 0) {
                len = lps[len - 1];
            }
            else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

/*
 * KMP algorithm
 */
int cooler_strstr(const char *haystack, const char *needle)
{
    int haystack_len = strlen(haystack);
    int needle_len   = strlen(needle);
    int *lps         = NULL;
    bool lps_alloc   = false;
    if (needle_len <= SHORT_STR_LEN) {
        int lps_stack[SHORT_STR_LEN * sizeof(int)] = {};
        lps                                        = lps_stack;
    }
    else {
        lps       = cooler_malloc(sizeof(*lps) * needle_len);
        lps_alloc = true;
    }

    str_search_compute_lps(needle, needle_len, lps);

    int i = 0;
    int j = 0;
    while (i < haystack_len && i < needle_len) {
        if (needle[j] == haystack[i]) {
            j++;
            i++;
        }
        if (j == needle_len) {
            if (lps_alloc) {
                free(lps);
            }
            return i - j;
        }
        else if (i < haystack_len && needle[j] != haystack[i]) {
            if (j != 0) {
                j = lps[j - 1];
            }
            else {
                i++;
            }
        }
    }
    if (lps_alloc) {
        free(lps);
    }
    return -1;
}

#ifndef _WIN32
// TODO: optimise test and verify
char *itoa(int value, char *str, int base)
{
    int i           = 0;
    int is_negative = 0;

    if (value == 0) {
        str[i++] = '0';
        str[i]   = '\0';
        return str;
    }
    if (value < 0 && base == 10) {
        is_negative = 1;
        value       = -value;
    }
    while (value != 0) {
        int remainder = value % base;
        str[i++] = (remainder > 9) ? (remainder - 10) + 'a' : remainder + '0';
        value    = value / base;
    }
    if (is_negative) {
        str[i++] = '-';
    }
    str[i] = '\0';
    str_rev(str);

    return str;
}
#endif

int ends_with(
    const char *str,
    const char *suffix,
    size_t str_len,
    size_t suffix_len)
{
    if (!str || !suffix)
        return 0;
    if (suffix_len > str_len)
        return 0;
    return strncmp(str + str_len - suffix_len, suffix, suffix_len) == 0;
}

int starts_with(
    const char *str,
    const char *prefix,
    size_t str_len,
    size_t prefix_len)
{
    if (!str || !prefix)
        return 0;
    if (prefix_len > str_len)
        return 0;
    return strncmp(str, prefix, prefix_len) == 0;
}

size_t
count_chars_before_string(const char *str, const char *before, size_t str_len)
{
    if (!before)
        return str_len;
    char *pos = strstr(str, before);
    if (!pos)
        return str_len;
    return pos - str;
}

size_t count_chars_after_string(
    const char *str,
    const char *after,
    size_t str_len,
    size_t after_len)
{
    if (!after)
        return str_len;
    char *pos = strstr(str, after);
    if (!pos)
        return str_len;
    return pos - str + after_len - str_len;
}

const char *find_nonspace(const char *begin, const char *end)
{
    if (NULL == end)
        end = begin + strlen(end);
    const char *res = begin;
    while (res < end && *res != '\0' && isspace((unsigned char)*res))
        res++;
    return res;
}

const char *find_space(const char *begin, const char *end)
{
    if (NULL == end)
        end = begin + strlen(end);
    const char *res = begin;
    while (res < end && *res != '\0' && !isspace((unsigned char)*res))
        res++;
    return res;
}

#define INTERNAL_BSIZE 32
static char idc[INTERNAL_BSIZE];
char *lltoa_helper(long long val)
{
    snprintf(idc, INTERNAL_BSIZE, "%lld", val);
    return strdup(idc);
}

char *sizetoa_helper(size_t val)
{
    snprintf(idc, INTERNAL_BSIZE, "%zu", (unsigned long long)val);
    return strdup(idc);
}
#undef INTERNAL_BSIZE

long long strtoll_helper(char *str)
{
    char *endstr = (char *)find_space(str, str + strlen(str));
    return strtoll(str, &endstr, 10);
}

size_t strtoull_helper(char *str)
{
    char *endstr = (char *)find_space(str, str + strlen(str));
    return (size_t)strtoull(str, &endstr, 10);
}

long long strtoll_helper_2(char *str, char *endstr)
{
    endstr = ptr_coallesce(endstr, (void *)find_space(str, endstr));
    return strtoll(str, &endstr, 10);
}

size_t strtoull_helper_2(char *str, char *endstr)
{
    endstr = ptr_coallesce(endstr, (void *)find_space(str, endstr));
    return (size_t)strtoull(str, &endstr, 10);
}

int strarr_cmp(char **arra, char **arrb, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        if (strcmp(arra[i], arrb[i]))
            return 1;
    }
    return 0;
}

int char_in_str(char ch, const char *ctrl)
{
    size_t n = strlen(ctrl);
    for (size_t i = 0; i < n; i++) {
        if (ch == ctrl[i])
            return 1;
    }
    return 0;
}

#ifdef _WIN32
char *strsep(char **stringp, const char *delim)
{
    char *start = *stringp;
    char *ptr;

    if (!start)
        return NULL;
    if (!*delim) {
        ptr = start + strlen(start);
    }
    else {
        ptr = strpbrk(start, delim);
        if (!ptr) {
            *stringp = NULL;
            return start;
        }
    }
    *ptr     = '\0';
    *stringp = ptr + 1;
    return start;
}
#else
char *strpbrk(const char *str, const char *ctrl)
{
    const char *res = str;
    while (*(res) != '\0') {
        if (char_in_str(*res, ctrl))
            return (char *)res;
        res++;
    }
    return NULL;
}
#endif

int count_for_split_str_by_whitespace(char *str, size_t str_len)
{
    int res   = 0;
    char *end = str + str_len;
    char *sp  = str;
    while (sp != end && isspace((unsigned char)*sp))
        sp++;
    while (sp != end) {
        res++;
        while (sp != end && !isspace((unsigned char)*sp))
            sp++;
        while (sp != end && isspace((unsigned char)*sp))
            sp++;
    }
    return res;
}

int split_string_by_whitespace(char *str, char **out, size_t str_len, int count)
{
    char *end = str + str_len;
    char *sp  = str;
    int i     = 0;
    sp        = (char *)find_nonspace(sp, end);
    while (sp != end && i < count) {
        char *sp2 = sp;
        sp2       = (char *)find_space(sp2, end);
        out[i]    = stralloc(sp2 - sp + 1);
        memcpy(out[i], sp, sp2 - sp);
        i++;
        sp = sp2;
        sp = (char *)find_nonspace(sp, end);
    }
    return i;
}

char *str_replace(const char *orig, const char *rep, const char *with)
{
    if (!orig || !rep)
        return NULL;
    size_t len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL;
    if (!with)
        with = "";
    size_t len_with = strlen(with);
    size_t len_orig = strlen(orig);

    const char *ins = orig;
    char *tmp;
    int count = 0;
    for (; (tmp = strstr(ins, rep)); ++count) {
        ins = tmp + len_rep;
    }

    char *result;
    tmp = result =
        cooler_malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    size_t len_front = 0;
    while (count--) {
        ins       = strstr(orig, rep);
        len_front = ins - orig;
        strncpy_s(tmp, len_orig + 1, orig, len_front);
        tmp += len_front;
        strcpy_s(tmp, len_with + 1, with);
        tmp += len_with;
        orig += len_front + len_rep;
    }
    strcpy_s(tmp, len_orig + 1, orig);
    return result;
}
