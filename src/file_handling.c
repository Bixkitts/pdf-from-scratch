#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
    #include <io.h>
#else
    #include <sys/mman.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <unistd.h>
#endif

#include "file_handling.h"
#include "mem_utils.h"

#ifdef _WIN32
void *memory_map_file(const char *filename, size_t *file_size)
{
    HANDLE hFile = CreateFileA(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        perror("Error opening file");
        return NULL;
    }

    *file_size = GetFileSize(hFile, NULL);
    HANDLE hMapping = CreateFileMappingA(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if (hMapping == NULL) {
        perror("Error creating file mapping");
        CloseHandle(hFile);
        return NULL;
    }

    void *file_content = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
    if (file_content == NULL) {
        perror("Error mapping view of file");
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return NULL;
    }

    CloseHandle(hMapping);
    CloseHandle(hFile);

    return file_content;
}

void unmap_file(void *file_content, size_t file_size)
{
    UnmapViewOfFile(file_content);
}
#else

void *fail_with(const char *message, int fd) {
    perror(message);
    if(fd != -1) close(fd);
    return NULL;
}

void *memory_map_file(const char *filename, size_t *file_size)
{
    int fd = open(filename, O_RDONLY);
    struct stat sb;
    void *file_content;
    if (fd == -1) {
        return fail_with("Error opening file", fd);
    }
    if (fstat(fd, &sb) == -1) {
        return fail_with("Error getting file size", fd);
    }
    *file_size = sb.st_size;
    *file_content = mmap(NULL, *file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file_content == MAP_FAILED) {
        return fail_with("Error mapping file", fd);
    }
    close(fd);
    return file_content;
}

void unmap_file(void *file_content, size_t file_size)
{
    munmap(file_content, file_size);
}
#endif

// Function to read file contents and find lines starting with '#'
struct line_info* parse_file(char *file_content, size_t file_size, size_t *num_lines)
{
    size_t capacity = 10; 
    struct line_info *lines = cooler_malloc(capacity * sizeof(*lines));

    size_t line_index = 0;
    char *line_start = file_content;
    for (size_t i = 0; i < file_size; i++) {
        if (file_content[i] == '\n' || i == file_size - 1) {
            size_t line_length = &file_content[i] - line_start + 1;

            if (line_start[0] == '#') {
                if (*num_lines >= capacity) {
                    capacity *= 2;
                    lines = cooler_realloc(lines, capacity * sizeof(*lines));
                }
                
                lines[*num_lines].line_index = line_index;
                lines[*num_lines].line_length = line_length;
                (*num_lines)++;
            }

            line_index++;
            line_start = &file_content[i + 1];
        }
    }

    return lines;
}

int write_buffer_to_file(const char *filename, const char *data)
{
    FILE *file = fopen(filename, "w+");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    size_t buffer_size = strlen(data);

    size_t written_size = fwrite(data, sizeof(char), buffer_size, file);
    if (written_size != buffer_size) {
        perror("Error writing to file");
        fclose(file);
        return -1;
    }

    fclose(file);

    return 0;
}
