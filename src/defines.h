#pragma once

// strdup() feature test macro
#ifndef _WIN32
#define _POSIX_C_SOURCE 200809L
#endif

#ifndef _WIN32
#define strcpy_s(a, b, c) strcpy(a, c)
#define strncpy_s(a, b, c, d) strncpy(a, c, d)
#define _itoa_s(a, b, c, d) itoa(a, b, d)
#endif
