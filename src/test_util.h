#pragma once

#include <assert.h>
#include <stdio.h>

#ifdef SOFT_FAIL
	void iassert(int a) { if(!a) printf("assertion failure :(\n"); }
#else
#define iassert(a) assert(a)
#endif

#define STARRLEN(starr) sizeof(starr)/sizeof(starr[0])
static int p = 0;
static int t = 0;
static int tres = 0;
#define RUNTEST(x) {tres = x(); iassert(tres); p += tres; t++;}; 

#undef SOFT_FAIL
