#pragma once

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include <stdio.h>
#ifdef _WIN32
#include <corecrt_malloc.h>
#endif

#include "mem_utils.h"
/*
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
*/
static int is_left_dd(const char *cp) {
   return *(uint16_t*)cp == ('<' | ('<' << 8));
}
static int is_right_dd(const char *cp) {
   return *(uint16_t*)cp == ('>' | ('>' << 8));
}

int find_nested_in_dict_str(const char *str_dict, size_t str_len) {
    if(str_len < 4) return 0;
	const char *s_ptr = str_dict + str_len - 3;
	const char *o_ptr = NULL;

	while(s_ptr != str_dict && !is_left_dd(s_ptr)) {
		if(is_right_dd(s_ptr)) o_ptr = s_ptr;
		s_ptr--;
	};
	printf("%p %p \n", (void*)s_ptr, (void*)o_ptr);
	if(s_ptr == str_dict && !is_left_dd(s_ptr)) return 0;
	if(o_ptr == NULL) return 0;
	s_ptr += 2;
	size_t out_len = o_ptr - s_ptr;
	char* out = cooler_malloc(out_len + 1);
	out[out_len] = '\0';
	memcpy(out, s_ptr, out_len);
	printf("%s \n", out);
	return 1;
}

int str_dict_to_arr(char *str_dict, char **out) {
	size_t dict_len = strlen(str_dict);
	char *begin = &str_dict[2];
	int cnt = 0;
	while (*(begin - 1) != '<' && *(begin - 2) != '<') begin++;
	
	char* end = strstr(begin, ">>");
	if (!end) end = str_dict + dict_len;
	char* sp = strstr(begin, " ");
	char* nl = strstr(begin, "\n");
	if(!sp) return 0;

	return cnt;
}
