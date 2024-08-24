#pragma once

#include <string.h>

#include "test_util.h"
#include "dict_util.h"
#include "defines.h"

int test_str_dict_to_arr(void) {
	const char* ex_dict2 = "<<"
		"/Item1 0.4"
		"/Item2 true"
		"/LastItem (not !)"
		"/VeryLastItem (OK)"
		">>";

	char** out = NULL;
	int items = str_dict_to_arr(strdup(ex_dict2), out);
	for (int i = 0; i < items; i++) {
		if (NULL != out) printf("%s \n", out[i]);
	}
	return 1;
}

int test_find_nested_in_dict_str(void) {
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
	find_nested_in_dict_str(out_dict, strlen(out_dict));
	return 1;
}

void do_dict_util_tests(void) {
	RUNTEST(test_str_dict_to_arr);
	RUNTEST(test_find_nested_in_dict_str);
}
