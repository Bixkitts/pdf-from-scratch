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
int build_pdf_from_md (const char *md,
                       size_t md_size,
                       struct pdf *out_pdf);

void write_catalog(struct pdf *out_pdf);
void write_pages_catalog(struct pdf *out_pdf);
void write_page_dictionary(struct pdf *out_pdf);
void write_font_dictionary(struct pdf *out_pdf);
void write_object_offsets(struct pdf *out_pdf);
void write_text_stream(struct pdf *out_pdf);