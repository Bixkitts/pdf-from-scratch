#pragma once

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "defines.h"
#include "str_utils.h"
#include "mem_utils.h"

#define get_obj_length1(obj_strarr) get_obj_length(obj_strarr, STARRLEN(obj_strarr), NULL)
#define get_obj_length2(obj_strarr, len) get_obj_length(obj_strarr, len, NULL)

size_t get_obj_length(const char *obj_strarr[], size_t len, size_t lengths[]) {
	size_t totalLength = 0;
	for (size_t i = 0; i < len; i++) {
		size_t clen = strlen(obj_strarr[i]);
		totalLength += clen;
		if(lengths) lengths[i] = clen;
	}
	return totalLength;
}

char *get_with_delim(const char *str) {
	const char *replace_left_delim = "{{{";
	const size_t ldlen = sizeof("{{{") - 1;
	const char *replace_right_delim = "}}}";
	const size_t rdlen = sizeof("}}}") - 1;
	size_t llen = strlen(str);
	char *c_label = stralloc(ldlen + rdlen + llen + 1);
	strcpy_s(c_label, ldlen + 1, replace_left_delim);
	strcpy_s(c_label + ldlen, llen + 1, str);
	strcpy_s(c_label + ldlen + llen, rdlen + 1, replace_right_delim);

	return c_label;
}

int replace_label_in_obj(char *obj_strarr[], int len, const char *label, const char *replace) {
	char *c_label = get_with_delim(label);
	if (c_label == 0) return 0;
	char *out = NULL;
	int cnt = 0;
	
	for (int i = 0; i < len; i++) {
		if ((out = str_replace(obj_strarr[i], c_label, strdup(replace)))) {
			obj_strarr[i] = out;
			cnt++;
		}
	}
	return cnt;
}

size_t get_stream_length(const char* obj_strarr[], int len) {
	static const char *ENDSTREAM = "\nendstream\n";
	static const char *STREAM = "stream\n";
	size_t total_length = 0;
	int index_has_stream = -1;
	int index_has_endstream = -1;
	char* tmp = NULL;
	for (int i = 0; i < len; i++) {
		const char *bit = obj_strarr[i];
		if ((tmp = strstr(bit, STREAM)) && index_has_stream == -1) {
			index_has_stream = i;
		}
		if ((tmp = strstr(bit, ENDSTREAM)) && index_has_endstream == -1) {
			index_has_endstream = i;
		}
	}
	if (index_has_stream == -1 || index_has_endstream == -1) return 0;
	if (index_has_stream > index_has_endstream) return 0;
	if (index_has_stream == index_has_endstream) return 0; //TODO implement

	const char *bit = obj_strarr[index_has_stream];
	total_length += count_chars_after_string(bit, STREAM, strlen(bit), sizeof(STREAM) - 1);

	for (int i = index_has_stream + 1; i < index_has_endstream; i++) {
		bit = obj_strarr[i];
		size_t count_b = count_chars_before_string(bit, ENDSTREAM, strlen(bit));
		total_length += count_b;
	}
	return total_length;
}

size_t obj_join(char **out, const char *obj_strarr[], size_t len) {
	size_t *lens = cooler_malloc(sizeof(size_t) * len);
	size_t obj_len = get_obj_length(obj_strarr, len, lens);
	*out = stralloc(obj_len + 1); 
	for (size_t i = 0, rsum = 0; i < len; i++) {
		strcpy_s(*out + rsum, lens[i] + 1, obj_strarr[i]);
		rsum += lens[i];
	}
	return obj_len;
}

const char *obsraj = "0000000000 65535 f";

const char *find_cross_reference_section(const char *file_content, size_t content_len) {
	const char *res = strstr(file_content, "\nxref\n") + 1;
	if(res > file_content + content_len) return NULL;
	return res;
}

const char *find_trailer(const char *xref_tag_ptr, const char *eof) {
	const char *res = strstr(xref_tag_ptr, "\ntrailer\n") + 1;
	if(res > eof) return NULL;
	return res;
}

struct xref_section {
	const char *xref_tag;
	const char *trailer_tag;
};

struct xref_section get_xref_section(const char *file_content, size_t content_len) {
	struct xref_section res;
	res.xref_tag = find_cross_reference_section(file_content, content_len);
	res.trailer_tag = find_trailer(res.xref_tag, file_content + content_len);
	return res;
}

struct subsection_heading {
	size_t first_obj_num;
	size_t obj_cnt;
	const char *begin;
	const char *end;
};

static const char *get_section_end(const char *ptr, size_t obj_cnt) {
	size_t i = obj_cnt;
	while (i--) {
		ptr = strstr(ptr, "\n");
	}
	return ptr;
}

struct subsection_heading get_first_section(const char *xref_tag_ptr, const char *eof) {
	struct subsection_heading res;
	res.begin = xref_tag_ptr + 5;
	const char *nl = strstr(res.begin, "\n");
	const char *sp = strstr(res.begin, " ");
#ifndef SOFT_FAIL
	if(sp > nl) exit(1);
	if (res.begin > eof) exit(1);
#endif
	res.first_obj_num = strtoull_helper((char*)res.begin);
	res.obj_cnt = strtoull_helper((char*)sp + 1);
	res.end = get_section_end(nl, res.obj_cnt);
	return res;
}

int has_next_section(struct subsection_heading section, const char *trailer) {
	const char *se = section.end;
	return (se + 1) == trailer;
}

struct subsection_heading get_next_section(struct subsection_heading section, const char *eof) {
	struct subsection_heading res;
	res.begin = section.end + 1;
	const char *nl = strstr(res.begin, "\n");
	const char *sp = strstr(res.begin, " ");
#ifndef SOFT_FAIL
	if (sp > nl) exit(1);
	if (res.begin > eof) exit(1);
#endif
	res.first_obj_num = strtoull_helper((char*)res.begin);
	res.obj_cnt = strtoull_helper((char*)sp + 1);
	res.end = get_section_end(nl, res.obj_cnt);
	return res;
}

int has_free_objects(struct subsection_heading sec) {
	char *nl = strstr(sec.begin, "\n");
	return strncmp(nl + 1, "0000000000 65535 f", 18) == 0;
}