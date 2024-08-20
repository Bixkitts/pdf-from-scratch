#pragma once

#include "defines.h"
#include "str_utils.h"
#include "test_util.h"
#include <stdio.h>

int test_count_for_split_str_by_whitespace(void) {
	const char* test_str = "a    b c d   e\n f\t\n\v g";
	int res = count_for_split_str_by_whitespace(strdup(test_str), strlen(test_str));

	return res == 7;
}

int test_split_string_by_whitespace(void) {
	char* test_str = strdup("a    b c d   e\n f\t\n\v g");
	int cnt = count_for_split_str_by_whitespace(strdup(test_str), strlen(test_str));
	char **strs = (char**)malloc(sizeof(char*) * cnt);
	if(strs == NULL) assert(0);
	
	split_string_by_whitespace(test_str, strs, strlen(test_str), cnt);
	return strcmp(strs[6], "g") == 0;
}

void do_str_utils_tests(void) {
	RUNTEST(test_count_for_split_str_by_whitespace);
	RUNTEST(test_split_string_by_whitespace);
}
