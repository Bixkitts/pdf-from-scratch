#pragma once

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#include "defines.h"
#include "mem_utils.h"

// TODO: optimise test and verify
void str_rev(char *str)
{
    size_t length = strlen(str);
    for (size_t i = 0; i < length / 2; i++) {
        char temp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = temp;
    }
}

#ifndef _WIN32
// TODO: optimise test and verify
char* itoa(int value, char* str, int base)
{
    int i = 0;
    int is_negative = 0;

    if (value == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
    if (value < 0 && base == 10) {
        is_negative = 1;
        value = -value;
    }
    while (value != 0) {
        int remainder = value % base;
        str[i++] = (remainder > 9) ? (remainder - 10) + 'a' : remainder + '0';
        value = value / base;
    }
    if (is_negative) {
        str[i++] = '-';
    }
    str[i] = '\0';
    str_rev(str);

    return str;
}
#endif


int ends_with(const char *str, const char *suffix, size_t str_len, size_t suffix_len) {
	if (!str || !suffix) return 0;
	if (suffix_len > str_len) return 0;
	return strncmp(str + str_len - suffix_len, suffix, suffix_len) == 0;
}

int starts_with(const char *str, const char *prefix, size_t str_len, size_t prefix_len) {
	if (!str || !prefix) return 0;
	if (prefix_len > str_len) return 0;
	return strncmp(str, prefix, prefix_len) == 0;
}

size_t count_chars_before_string(const char *str, const char *before, size_t str_len) {
	if (!before) return str_len;
	char* pos = strstr(str, before);
	if(!pos) return str_len;
	return pos - str;
}

size_t count_chars_after_string(const char *str, const char *after, size_t str_len, size_t after_len) {
	if (!after) return str_len;
	char *pos = strstr(str, after);
	if (!pos) return str_len;
	return pos - str + after_len - str_len;
}

static char idc[32];
char *itoa_helper(int val) {
	_itoa_s(val, idc, 32, 10);
	return strdup(idc);
}

int strarr_cmp(char **arra, char **arrb, size_t len) {
	for (size_t i = 0; i < len; i++) {
		if(strcmp(arra[i], arrb[i])) return 1;
	}
	return 0;
}

// TODO: Linux version
#ifdef _WIN32
char *strsep(char **stringp, const char *delim) {
	char *start = *stringp;
	char *ptr;
	
	if (!start) return NULL;
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
	*ptr = '\0';
	*stringp = ptr + 1;
	return start;
}
#endif

int count_for_split_str_by_whitespace(char *str, size_t str_len) {
	int res = 0;
	char *end = str + str_len;
	char *sp = str;
	while (sp != end && isspace((unsigned char)*sp)) sp++;
	while (sp != end) {
		res++;
		while (sp != end && !isspace((unsigned char)*sp)) sp++;
		while (sp != end && isspace((unsigned char)*sp)) sp++;
	}
	return res;
}

int split_string_by_whitespace(char *str, char **out, size_t str_len, int count) {
	char *end = str + str_len;
	char *sp = str;
	int i = 0;
	while (sp != end && isspace((unsigned char)*sp)) sp++;
	while (sp != end && i < count) {
		char *sp2 = sp;
		while (sp2 != end && !isspace((unsigned char)*sp2)) sp2++;
		out[i] = cooler_malloc(sp2 - sp + 1);
		memcpy(out[i], sp, sp2 - sp);
		out[i][sp2 - sp] = '\0';
		i++;
		sp = sp2;
		while (sp != end && isspace((unsigned char)*sp)) sp++;
	}
	return i;
}

char *str_replace(char *orig, char *rep, const char *with) {
	if (!orig || !rep) return NULL;
	size_t len_rep = strlen(rep);
	if (len_rep == 0) return NULL;
	if (!with) with = "";
	size_t len_with = strlen(with);
	size_t len_orig = strlen(orig);

	char* ins = orig;
	char* tmp;
	int count = 0;
	for (; (tmp = strstr(ins, rep)); ++count) {
		ins = tmp + len_rep;
	}

	char* result;
	tmp = result = cooler_malloc(strlen(orig) + (len_with - len_rep) * count + 1);

	size_t len_front = 0;
	while (count--) {
		ins = strstr(orig, rep);
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

