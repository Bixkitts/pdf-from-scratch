#pragma once

// strdup() feature test macro
#ifndef _WIN32
#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif
#endif

#ifndef _WIN32
#define strcpy_s(a, b, c) strcpy(a, c)
#define strncpy_s(a, b, c, d) strncpy(a, c, d)
#define _itoa_s(a, b, c, d) itoa(a, b, d)
#endif