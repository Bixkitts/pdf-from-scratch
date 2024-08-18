#include <string.h>

size_t get_obj_length(const char *obj_strarr[], int len) {
	size_t totalLength = 0;
	for (int i = 0; i < len; i++) {
		totalLength += strlen(obj_strarr[i]);
	}
	return totalLength;
}

static const char* replace_left_delim = "{{{";
static const char* replace_right_delim = "}}}";

int replace_label_in_obj(const char *obj_strarr[], int len, const char *label, const char *replace) {
	for (int i = 0; i < len; i++) {

	}
}