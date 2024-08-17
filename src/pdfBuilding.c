#include "pdfBuilding.h"

// Pdf creation helpers.
// Each one writes a different object
// to the pdf.
static void write_catalog         (struct Pdf *out_pdf);
static void write_pages_catalog   (struct Pdf *out_pdf);
static void write_page_dictionary (struct Pdf *out_pdf);
static void write_font_dictionary (struct Pdf *out_pdf);
static void write_object_offsets  (struct Pdf *out_pdf);
static void write_text_stream     (struct Pdf *out_pdf);

// return the size of the pdf and -1 on error
int build_pdf_from_md(const char *md, size_t md_size, struct Pdf *out_pdf)
{
    size_t pdf_capacity = md_size * 2;
    // need size of md + size of pdf metadata
    out_pdf->data = calloc(pdf_capacity, 1);

    // Call the pdf creation helpers here

    return out_pdf->len;
}

static void write_catalog(struct Pdf *out_pdf)
{
}

static void write_pages_catalog(struct Pdf *out_pdf)
{
}

static void write_page_dictionary(struct Pdf *out_pdf)
{
}

static void write_font_dictionary(struct Pdf *out_pdf)
{
}

static void write_object_offsets(struct Pdf *out_pdf)
{
}

static void write_text_stream(struct Pdf *out_pdf)
{
}
