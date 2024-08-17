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
int build_pdf_from_md(const char *md, size_t size, struct Pdf *out_pdf);
