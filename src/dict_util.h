#pragma once

#include <string.h>
#include <assert.h>
#include <stdio.h>

const char *ex_dict = "<</Type / Example"
"/Subtype /DictionaryExample"
"/Version 0.01"
"/IntegerItem 12"
"/StringItem(a string)"
"/Subdictionary"
	"<<"
"/Item1 0.4"
"/Item2 true"
"/LastItem (not !)"
"/VeryLastItem (OK)"
	">>"
">>";

const char *out_dict = "<</Type / Example"
"/Subtype /DictionaryExample"
"/Version 0.01"
"/IntegerItem 12"
"/StringItem(a string)"
"/Subdictionary"
"{{{tagXX}}} r"
">>"
"{{{tagXX}}} obj"
"<<"
"/Item1 0.4"
"/Item2 true"
"/LastItem (not !)"
"/VeryLastItem (OK)"
">>";

const char *ex_dict2 = "<<"
"/Item1 0.4"
"/Item2 true"
"/LastItem (not !)"
"/VeryLastItem (OK)"
">>";

const char *exout[][2] = {
{"/Item1","0.4"},
{"/Item2","true"},
{"/LastItem","(not !)"},
{"/VeryLastItem","(OK)"}
};

int str_dict_to_arr(char* str_dict, char** out) {
	size_t dict_len = strlen(str_dict);
	char* begin = &str_dict[2];
	while (*(begin - 1) != '<' && *(begin - 2) != '<') begin++;
	
	char* end = strstr(begin, ">>");
	if (!end) return 0;
	
}

int test_str_dict_to_arr() {
	char** out;
	int items = str_dict_to_arr(strdup(ex_dict2), out);
	for (int i = 0; i < items; i++) {
		printf("%s \n", out[i]);
	}
	return 1;
}

#define STARRLEN(starr) sizeof(starr)/sizeof(starr[0])
static int p = 0;
static int t = 0;
static int tres = 0;
#define RUNTEST(x) {tres = x(); assert(tres); p += tres; t++;}; 

int do_dict_util_tests() {
	RUNTEST(test_str_dict_to_arr);
}