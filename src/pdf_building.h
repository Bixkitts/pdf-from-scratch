#include <stdlib.h>

struct pdf {
    char   *data;
    size_t  len;
    size_t  capacity;
};

/* Should dynamically allocate a buffer
 * to hold the markdown in pdf form and
 * return a pointer to it.
 */
size_t build_pdf_from_md (const char *md,
                       size_t md_size,
                       struct pdf *out_pdf);
