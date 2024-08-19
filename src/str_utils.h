#pragma once

#include <string.h>
#include <stdlib.h>
#include <assert.h>

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

int count_chars_before_string(const char *str, const char *before, size_t str_len) {
	if (!before) return str_len;
	char* pos = strstr(str, before);
	if(!pos) return str_len;
	return pos - str;
}

int count_chars_after_string(const char *str, const char *after, size_t str_len, size_t after_len) {
	if (!after) return str_len;
	char *pos = strstr(str, after);
	if (!pos) return str_len;
	return pos - str + after_len - str_len;
}

static char idc[32];
char *itoa_helper(int val) {
	itoa(val, idc, 10);
	return strdup(idc);
}

int strarr_cmp(char **arra, char **arrb, size_t len) {
	for (size_t i = 0; i < len; i++) {
		if(strcmp(arra[i], arrb[i])) return 1;
	}
	return 0;
}

#ifdef _WIN32
char* strsep(char **stringp, const char *delim) {
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
		out[i] = (char*)malloc(sp2 - sp + 1);
		if(out[i] == NULL) assert(0);
		memcpy_s(out[i], sp2 - sp, sp, sp2 - sp);
		out[i][sp2 - sp] = '\0';
		i++;
		sp = sp2;
		while (sp != end && isspace((unsigned char)*sp)) sp++;
	}
	return i;
}