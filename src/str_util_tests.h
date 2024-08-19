#pragma once

#include "str_utils.h"
#include "test_util.h"
#include <stdio.h>

int test_count_for_split_str_by_whitespace() {
	const char* test_str = "a    b c d   e\n f\t\n\v g";
	int res = count_for_split_str_by_whitespace(strdup(test_str), strlen(test_str));

	return res == 7;
}

int test_split_string_by_whitespace() {
	const char* test_str = "a    b c d   e\n f\t\n\v g";
	int cnt = count_for_split_str_by_whitespace(strdup(test_str), strlen(test_str));
	char **strs = (char**)malloc(sizeof(char*) * cnt);
	if(strs == NULL) assert(0);
	
	int res = split_string_by_whitespace(test_str, strs, strlen(test_str), cnt);
	for (int i = 0; i < cnt; i++) {
		printf("%s \n", strs[i]);
	}
	return strcmp(strs[6], "g") == 0;
}

int do_str_utils_tests() {
	RUNTEST(test_count_for_split_str_by_whitespace);
	RUNTEST(test_split_string_by_whitespace);
}