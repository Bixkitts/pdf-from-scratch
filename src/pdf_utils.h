#include <string.h>

size_t get_obj_length(const char* obj_strarr[], int len) {
	size_t totalLength = 0;
	for (int i = 0; i < len; i++) {
		totalLength += strlen(obj_strarr[i]);
	}
	return totalLength;
}