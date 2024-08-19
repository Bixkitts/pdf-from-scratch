#pragma once

#include "test_util.h"
#include "dict_util.h"

int test_str_dict_to_arr() {
	char** out;
	int items = str_dict_to_arr(strdup(ex_dict2), out);
	for (int i = 0; i < items; i++) {
		printf("%s \n", out[i]);
	}
	return 1;
}

int do_dict_util_tests() {
	RUNTEST(test_str_dict_to_arr);
}