#include <stddef.h>
#include <string.h>

#include "defines.h"

#define SSTRE 24

struct sstr {
	char content[SSTRE - 1 - sizeof(unsigned char)];
	char nt;
	unsigned char len : sizeof(unsigned char) * 8 - 1;
	unsigned char is_long : 1;
};

struct lstr {
	char *content;
	size_t capacity;
	size_t len : sizeof(size_t) * 8 - 1;
	size_t is_long : 1;
};

union ustr {
	struct sstr sstr;
	struct lstr lstr;
	struct unkw {
		char unkw[sizeof(struct lstr) - sizeof(unsigned char)];
		unsigned char pad : 7;
		unsigned char is_long : 1;
	} unkw;
};

union ustr create(char *str, size_t len, size_t capacity) {
	if(len == 0) len = strlen(str);
	union ustr res;
	if ((res.unkw.is_long = len > (SSTRE - 1))) {
		res.lstr.len = len;
		res.lstr.content = str;
		res.lstr.capacity = capacity;
	}
	else {
		res.sstr.len = (unsigned char)len;
		memcpy_s(res.sstr.content, SSTRE - 2, str, len);
		res.sstr.nt = '\0';
	}
	return res;
}

int is_sstr_long(union ustr ustr) {
	return ustr.unkw.is_long;
}

size_t sstr_len(union ustr ustr) {
	return ustr.unkw.is_long ? ustr.lstr.len : ustr.sstr.len;
}
