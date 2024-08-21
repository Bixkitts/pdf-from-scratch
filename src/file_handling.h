#ifdef _WIN32
    #include <windows.h>
#else
    #include <fcntl.h>
#endif

#include <stdio.h>

/* Indexes to lines beginning with '#'
 * and their length
 */
struct line_info {
    size_t line_index;
    size_t line_length;
};

void             *memory_map_file    (const char *filename,
                                      size_t *file_size);
struct line_info *parse_file         (char *file_content,
                                      size_t file_size,
                                      size_t *num_lines);
int               write_data_to_file (const char *filename,
                                      const char *data);
void             unmap_file          (void *file_content,
                                      size_t file_size);

