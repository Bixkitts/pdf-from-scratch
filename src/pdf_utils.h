#pragma once

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "str_utils.h"

size_t get_obj_length(const char *obj_strarr[], int len) {
	size_t totalLength = 0;
	for (int i = 0; i < len; i++) {
		totalLength += strlen(obj_strarr[i]);
	}
	return totalLength;
}

char *str_replace(char *orig, char *rep, char *with) {
	char *result; 
	char *ins;
	char *tmp;
	int len_rep;
	int len_with;
	int len_front;
	int count;

	if (!orig || !rep) return NULL;
	len_rep = strlen(rep);
	if (len_rep == 0) return NULL;
	if (!with) with = strdup("");
	len_with = strlen(with);

	ins = orig;
	for (count = 0; (tmp = strstr(ins, rep)); ++count) {
		ins = tmp + len_rep;
	}

	tmp = result = (char*)malloc(strlen(orig) + (len_with - len_rep) * count + 1);

	if (!result) return NULL;

	while (count--) {
		ins = strstr(orig, rep);
		len_front = ins - orig;
		tmp = strncpy(tmp, orig, len_front) + len_front;
		tmp = strcpy(tmp, with) + len_with;
		orig += len_front + len_rep;
	}
	strcpy(tmp, orig);
	return result;
}


char* get_with_delim(char* str) {
	static const char* replace_left_delim = "{{{";
	static const size_t ldlen = sizeof("{{{") - 1;
	static const char* replace_right_delim = "}}}";
	static const size_t rdlen = sizeof("}}}") - 1;
	size_t llen = strlen(str);
	char* c_label = (char*)malloc(ldlen + rdlen + llen);
	if(c_label == 0) return NULL;
	strcpy(c_label, replace_left_delim);
	strcpy(c_label + ldlen, str);
	strcpy(c_label + ldlen + llen, replace_right_delim);

	return c_label;
}

int replace_label_in_obj(char *obj_strarr[], int len, const char *label, const char *replace) {
	char *c_label = get_with_delim(label);
	if (c_label == 0) return 0;
	char *out = NULL;
	int cnt = 0;
	
	for (int i = 0; i < len; i++) {
		if (out = str_replace(obj_strarr[i], c_label, strdup(replace))) {
			obj_strarr[i] = out;
			cnt++;
		}
	}
	return cnt;
}

size_t get_stream_length(const char* obj_strarr[], int len) {
	static const char* ENDSTREAM = "\nendstream\n";
	static const char* STREAM = "stream\n";
	size_t total_length = 0;
	int index_has_stream = -1;
	int index_has_endstream = -1;
	char* tmp = NULL;
	for (int i = 0; i < len; i++) {
		const char* bit = obj_strarr[i];
		if (tmp = strstr(bit, STREAM) && index_has_stream == -1) {
			index_has_stream = i;
		}
		if (tmp = strstr(bit, ENDSTREAM) && index_has_endstream == -1) {
			index_has_endstream = i;
		}
	}
	if (index_has_stream == -1 || index_has_endstream == -1) return 0;
	if (index_has_stream > index_has_endstream) return 0;
	if (index_has_stream == index_has_endstream) return -1; //TODO implement

	const char* bit = obj_strarr[index_has_stream];
	total_length += count_chars_after_string(bit, STREAM, strlen(bit), sizeof(STREAM) - 1);

	for (int i = index_has_stream + 1; i < index_has_endstream; i++) {
		const char* bit = obj_strarr[i];
		size_t count_b = count_chars_before_string(bit, ENDSTREAM, strlen(bit));
		total_length += count_b;
	}
	printf("slen %ld \n", (long)total_length);
	return total_length;
}

#define STARRLEN(starr) sizeof(starr)/sizeof(starr[0])

int obj_replace_test() {
	char* ex_obj[] = {
		"4 0 obj\n"
			"<<"
			"/Length {{{leee n}}}",
			"\n>>\n"
	"stream\n",
	"{{{leee n}}}",
	"\nendstream\n"
	};

	return replace_label_in_obj(ex_obj, STARRLEN(ex_obj), "leee n", "test") > 1;
}

int obj_stream_len_is_9() {
	char* ex_obj[] = {
		"4 0 obj\n"
			"<<"
			"/Length {{{leee n}}}",
			"\n>>\n"
	"stream\n",
	"AAAAAAAAA",
	"\nendstream\n"
	};
	size_t slen = get_stream_length(ex_obj, STARRLEN(ex_obj));
	return slen == 9;
}

int obj_stream_pastes_correctly() {
	char* ex_obj[] = {
			"4 0 obj\n"
				"<<"
				"/Length {{{leee n}}}",
				"\n>>\n"
		"stream\n",
		"AAAAAAAAA",
		"\nendstream\n"
	};
	size_t slen = get_stream_length(ex_obj, STARRLEN(ex_obj));
	char* lenstr = itoa_helper(slen);
	int res = replace_label_in_obj(ex_obj, STARRLEN(ex_obj), "leee n", lenstr);
	return strcmp(ex_obj[0], "4 0 obj\n"
				"<<"
		"/Length 9") == 0;
}

static int p = 0;
static int t = 0;
static tres = 0;
#define RUNTEST(x) tres = x(); assert(tres); p += tres; t++; 

int do_pdf_utils_tests() {
	RUNTEST(obj_replace_test);
	RUNTEST(obj_stream_len_is_9);
	RUNTEST(obj_stream_pastes_correctly);
	printf("passed %d of %d.\n", p, t);
}