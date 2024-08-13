#ifdef _WIN32
    #include <windows.h>
#else
    #include <fcntl.h>
#endif

struct LineInfo {
    size_t line_index;
    size_t line_length;
};

void            *memory_map_file (const char *filename,
                                  size_t *file_size);
void             unmap_file      (void *file_content,
                                  size_t file_size);
struct LineInfo* parse_file      (char *file_content,
                                  size_t file_size,
                                  size_t *num_lines);
