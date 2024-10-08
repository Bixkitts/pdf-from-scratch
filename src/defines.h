#pragma once

#ifndef _WIN32
#define strcpy_s(a, b, c)     strcpy(a, c)
#define strncpy_s(a, b, c, d) strncpy(a, c, d)
#define _itoa_s(a, b, c, d)   itoa(a, b, d)
#define _strdup(a)            strdup(a)
#define memcpy_s(a, b, c, d)  memcpy(a, c, d)
#define aligned_free(a)       free(a)
#endif

#ifdef _WIN32
#define strdup(a)           _strdup(a)
#define restrict            __restrict
#define aligned_alloc(a, b) _aligned_malloc(b, a)
#define aligned_free(a)     _aligned_free(a)
#endif
