#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

struct LineInfo {
    size_t line_index;
    size_t line_length;
};

// Function to read file contents and find lines starting with '#'
struct LineInfo* parse_file(char *file_content, size_t file_size, size_t *num_lines) {
    size_t capacity = 10; // Initial capacity for LineInfo array
    struct LineInfo *lines = malloc(capacity * sizeof(*lines));
    if (!lines) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    size_t line_index = 0;
    char *line_start = file_content;
    for (size_t i = 0; i < file_size; i++) {
        if (file_content[i] == '\n' || i == file_size - 1) {
            size_t line_length = &file_content[i] - line_start + 1;
            
            // Check if the line starts with '#'
            if (line_start[0] == '#') {
                if (*num_lines >= capacity) {
                    capacity *= 2;
                    lines = realloc(lines, capacity * sizeof(*lines));
                    if (!lines) {
                        perror("Failed to reallocate memory");
                        exit(EXIT_FAILURE);
                    }
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

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *filename = argv[1];

    // Open the file
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("Error getting file size");
        close(fd);
        exit(EXIT_FAILURE);
    }

    size_t file_size = sb.st_size;

    char *file_content = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file_content == MAP_FAILED) {
        perror("Error mapping file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);

    // Parse the file to get lines starting with '#'
    size_t num_lines = 0;
    struct LineInfo *lines = parse_file(file_content, file_size, &num_lines);

    // Output the array of line index and line length pairs
    printf("Found %zu lines starting with '#':\n", num_lines);
    for (size_t i = 0; i < num_lines; i++) {
        printf("Line %zu: Start Index = %zu, Length = %zu\n",
            i, lines[i].line_index, lines[i].line_length);
    }

    // Cleanup
    free(lines);
    if (munmap(file_content, file_size) == -1) {
        perror("Error unmapping file");
        exit(EXIT_FAILURE);
    }

    return 0;
}

