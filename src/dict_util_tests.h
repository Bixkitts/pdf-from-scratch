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

int test_find_nested_in_dict_str() {
	const char* out_dict = "<</Type / Example\n"
		"/Subtype /DictionaryExample\n"
		"/Version <<>>\n"
		"/IntegerItem 12\n"
		"/StringItem (a string)\n"
		"/Subdictionary "
		"{{{tagXX}}} r\n"
		">>\n"
		"{{{tagXX}}} obj\n"
		"<<\n"
		"/Item1 0.4\n"
		"/Item2 true\n"
		"/LastItem (not !)\n"
		"/VeryLastItem (OK)"
		">>>>>>>>>>>>>>>>>>>";
	int res = find_nested_in_dict_str(out_dict, strlen(out_dict));
	return 1;
}

int do_dict_util_tests() {
	RUNTEST(test_str_dict_to_arr);
	RUNTEST(test_find_nested_in_dict_str);
}