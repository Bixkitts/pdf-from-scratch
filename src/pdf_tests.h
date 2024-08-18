#pragma once

#include "pdf_utils.h"

#define STARRLEN(starr) sizeof(starr)/sizeof(starr[0])
static int p = 0;
static int t = 0;
static int tres = 0;
#define RUNTEST(x) {tres = x(); assert(tres); p += tres; t++;}; 

int obj_length_test() {
	char *ex_obj[] = {
		"4 0 obj\n"
			"<<"
			"/Length 9",
			"\n>>\n"
	"stream\n",
	"AAAAAAAAA",
	"\nendstream\n"
	};
	printf("tt %d \n", get_obj_length2(ex_obj, STARRLEN(ex_obj)));
	return get_obj_length2(ex_obj, STARRLEN(ex_obj)) == 50;
}

int obj_replace_test() {
	char *ex_obj[] = {
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
	char *ex_obj[] = {
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
	char *ex_obj[] = {
			"4 0 obj\n"
				"<<"
				"/Length {{{leee n}}}",
				"\n>>\n"
		"stream\n",
		"AAAAAAAAA",
		"\nendstream\n"
	};
	size_t slen = get_stream_length(ex_obj, STARRLEN(ex_obj));
	char *lenstr = itoa_helper(slen);
	int res = replace_label_in_obj(ex_obj, STARRLEN(ex_obj), "leee n", lenstr);
	return strcmp(ex_obj[0], "4 0 obj\n"
		"<<"
		"/Length 9") == 0;
}

int obj_lens_calculated_properly() {
	char* ex_arr[] = {
		"a",
		"b",
		"c"
	};
	size_t* lens = (size_t*)malloc(sizeof(size_t) * STARRLEN(ex_arr));
	get_obj_length(ex_arr, STARRLEN(ex_arr), lens);
		
	return lens[0] == 1 && lens[1] == 1 && lens[2] == 1;
}

int obj_lens_real_example_calculated_properly() {
	char* ex_obj[] = {
			"4 0 obj\n"
				"<<"
				"/Length {{{leee n}}}",
				"\n>>\n"
		"stream\n",
		"AAAAAAAAA",
		"\nendstream\n"
	};
	size_t* lens = (size_t*)malloc(sizeof(size_t) * STARRLEN(ex_obj));
	get_obj_length(ex_obj, STARRLEN(ex_obj), lens);

	return lens[3] == 11;
}

int obj_join_works() {
	char* ex_obj[] = {
			"4 0 obj\n"
				"<<"
				"/Length {{{leee n}}}",
				"\n>>\n"
		"stream\n",
		"AAAAAAAAA",
		"\nendstream\n"
	};
	char* out;
	obj_join(&out, ex_obj, STARRLEN(ex_obj));
	printf(out);
	return 1;
}

int do_pdf_utils_tests() {
	RUNTEST(obj_replace_test);
	RUNTEST(obj_stream_len_is_9);
	RUNTEST(obj_stream_pastes_correctly);
	RUNTEST(obj_length_test);
	RUNTEST(obj_lens_calculated_properly);
	RUNTEST(obj_lens_real_example_calculated_properly);
	RUNTEST(obj_join_works);
	printf("passed %d of %d in %s.\n", p, t, __FILE__);
}