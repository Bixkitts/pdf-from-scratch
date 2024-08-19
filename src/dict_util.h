#pragma once

#include <string.h>
#include <assert.h>
#include <stdio.h>

const char *ex_dict = "<</Type /Example\n"
"/Subtype /DictionaryExample\n"
"/Version <<>>\n"
"/IntegerItem 12\n"
"/StringItem (a string)\n"
"/Subdictionary "
	"<<"
"/Item1 0.4\n"
"/Item2 true\n"
"/LastItem (not !)\n"
"/VeryLastItem (OK)\n"
	">>"
">>";

const char *out_dict = "<</Type / Example\n"
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
">>";

const char *ex_dict2 = "<<"
"/Item1 0.4"
"/Item2 true"
"/LastItem (not !)"
"/VeryLastItem (OK)"
">>";

const char *exout[] = {
"/Item1","0.4",
"/Item2","true",
"/LastItem","(not !)",
"/VeryLastItem","(OK)"
};

static int is_left_dd(char* cp) {
	return *cp == '<' && *(cp + 1) == '<';
}
static int is_right_dd(char* cp) {
	return *cp == '>' && *(cp + 1) == '>';
}

int find_nested_in_dict_str(const char* str_dict) {
	const char* s_ptr = str_dict;
	char* o_ptr;
}

int str_dict_to_arr(char* str_dict, char** out) {
	size_t dict_len = strlen(str_dict);
	char* begin = &str_dict[2];
	int cnt = 0;
	while (*(begin - 1) != '<' && *(begin - 2) != '<') begin++;
	
	char* end = strstr(begin, ">>");
	if (!end) end = str_dict + dict_len;
	char* sp = strstr(begin, " ");
	char* nl = strstr(begin, "\n");
	if(!sp) return 0;

	return cnt;
}
