#pragma once

#include <string.h>

int ends_with(const char* str, const char* suffix, size_t str_len, size_t suffix_len) {
	if (!str || !suffix) return 0;
	if (suffix_len > str_len) return 0;
	return strncmp(str + str_len - suffix_len, suffix, suffix_len) == 0;
}

int starts_with(const char* str, const char* prefix, size_t str_len, size_t prefix_len) {
	if (!str || !prefix) return 0;
	if (prefix_len > str_len) return 0;
	return strncmp(str, prefix, prefix_len) == 0;
}

int count_chars_before_string(const char* str, const char* before, size_t str_len) {
	if (!before) return str_len;
	char* pos = strstr(str, before);
	if(!pos) return str_len;
	return pos - str;
}

int count_chars_after_string(const char* str, const char* after, size_t str_len, size_t after_len) {
	if (!after) return str_len;
	char* pos = strstr(str, after);
	if (!pos) return str_len;
	return pos + after_len - str - str_len;
}