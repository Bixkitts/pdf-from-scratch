#include <string.h>
#include <stdlib.h>
#include <assert.h>

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

int obj_replace_test() {
	char* ex_obj[] = {
		"4 0 obj\n"
			"<<"
			"/Length {{{leee n}}}",
			"\n>>\n"
	"stream\n",
	"{{{leee n}}}",
	"endstream\n"
	};

	return replace_label_in_obj(ex_obj, 2, "leee n", "test") > 1;
}