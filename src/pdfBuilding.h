#include <stdlib.h>

struct Pdf {
    char   *data;
    size_t  len;
    size_t  capacity;
};

/* Should dynamically allocate a buffer
 * to hold the markdown in pdf form and
 * return a pointer to it.
 */
struct Pdf *build_pdf_from_md(const char *md, size_t size);
