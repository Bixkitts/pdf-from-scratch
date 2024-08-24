
#include <stddef.h>
#include <string.h>

#define SSTRE 24

struct sstr {
	size_t len;
	char content[SSTRE-1];
	char nt;
};

struct lstr {
	size_t len;
	char *content;
	char padding[
		sizeof(struct sstr)
		- sizeof(size_t)
		- sizeof(char*)];
};

union ustr {
	struct sstr sstr;
	struct lstr lstr;
	struct unkw {
		size_t len;
		char unkw[sizeof(struct lstr) - sizeof(size_t)];
	} unkw;
};

union ustr create(char *str, size_t len) {
	if(len == 0) len = strlen(str);
	union ustr res;
	if (len < SSTRE) {
		res.sstr.len = len;
		memcpy_s(res.sstr.content, SSTRE - 1, str, len);
		res.sstr.nt = '\0';
	}
	else {
		res.lstr.len = len;
		res.lstr.content = str;
	}
	return res;
}