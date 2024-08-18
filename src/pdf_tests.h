#pragma once

#include "pdf_utils.h"

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
static int tres = 0;
#define RUNTEST(x) tres = x(); assert(tres); p += tres; t++; 

int do_pdf_utils_tests() {
	RUNTEST(obj_replace_test);
	RUNTEST(obj_stream_len_is_9);
	RUNTEST(obj_stream_pastes_correctly);
	printf("passed %d of %d in %s.\n", p, t, __FILE__);
}