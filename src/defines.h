#pragma once

#ifndef _WIN32
#define strcpy_s(a, b, c) strcpy(a, c)
#define strncpy_s(a, b, c, d) strncpy(a, c, d)
#define _itoa_s(a, b, c, d) itoa(a, b, d)
#define _strdup(a) strdup(a)
#endif

#ifdef _WIN32
#define strdup(a) _strdup(a)
#endif
