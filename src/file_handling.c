#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <io.h>
#include <windows.h>
#else
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#include "file_handling.h"
#include "mem_utils.h"

static int fail_with(const char *message, int fd);

#ifdef _WIN32
int memory_map_file(const char *dir, struct mapped_file *out_mf)
{
    HANDLE hFile = CreateFileA(
        dir,
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        perror("Error opening file");
        return -1;
    }

    out_mf->size = GetFileSize(hFile, NULL);
    HANDLE hMapping =
        CreateFileMappingA(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if (!hMapping) {
        perror("Error creating file mapping");
        CloseHandle(hFile);
        return -1;
    }

    out_mf->data = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
    if (!out_mf->data) {
        perror("Error mapping view of file");
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return -1;
    }

    CloseHandle(hMapping);
    CloseHandle(hFile);

    return 0;
}

void unmap_file(struct mapped_file *mf)
{
    UnmapViewOfFile(mf->data);
    mf->size = 0;
}
#else

static int fail_with(const char *message, int fd)
{
    perror(message);
    if (fd != -1)
        close(fd);
    return -1;
}

int memory_map_file(const char *dir, struct mapped_file *out_mf)
{
    int fd = open(dir, O_RDONLY);
    struct stat sb;
    if (fd == -1) {
        return fail_with("Error opening file", fd);
    }
    if (fstat(fd, &sb) == -1) {
        return fail_with("Error getting file size", fd);
    }
    out_mf->size = sb.st_size;
    out_mf->data = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (out_mf->data == MAP_FAILED) {
        return fail_with("Error mapping file", fd);
    }
    close(fd);
    return 0;
}

void unmap_file(struct mapped_file *mf)
{
    munmap(mf->data, mf->size);
    mf->size = 0;
}
#endif

int write_data_to_file(const char *data, size_t size, const char *fs_dir)
{
    FILE *new_file = fopen(fs_dir, "w+");
    if (!new_file) {
        perror("Error opening file");
        return -1;
    }
    size_t written_size = fwrite(data, sizeof(char), size, new_file);
    if (written_size != size) {
        perror("Error writing to file");
        fclose(new_file);
        return -1;
    }

    fclose(new_file);
    return 0;
}
