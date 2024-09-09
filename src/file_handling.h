#pragma once

#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#endif

#include <stdio.h>

#define FILE_MAP_FAIL   -1
#define FILE_WRITE_FAIL -1

struct mapped_file {
    void *data;
    size_t size;
};

int memory_map_file(const char *dir, struct mapped_file *out_mf);
int write_data_to_file(const char *data, size_t size, const char *dir);
void unmap_file(struct mapped_file *mf);
