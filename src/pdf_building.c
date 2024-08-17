#include "pdf_building.h"

// Strings we write into the PDF file.
// We _could_ replace this with something
// other than hardcoded strings in the binary.
const char *pdf_str_header = "%PDF-2.0\nPOOP";

// Pdf creation helpers.
// Each one writes a different object
// to the pdf.
static void write_catalog         (struct pdf *out_pdf);
static void write_pages_catalog   (struct pdf *out_pdf);
static void write_page_dictionary (struct pdf *out_pdf);
static void write_font_dictionary (struct pdf *out_pdf);
static void write_object_offsets  (struct pdf *out_pdf);
static void write_text_stream     (struct pdf *out_pdf);

// return the size of the pdf and -1 on error
int build_pdf_from_md(const char *md,
                      size_t md_size,
                      struct pdf *out_pdf)
{
    size_t pdf_capacity = md_size * 2;
    // need size of md + size of pdf metadata
    out_pdf->data = calloc(pdf_capacity, 1);

    // Call the pdf creation helpers here

    return out_pdf->len;
}

static void write_catalog(struct pdf *out_pdf)
{
}

static void write_pages_catalog(struct pdf *out_pdf)
{
}

static void write_page_dictionary(struct pdf *out_pdf)
{
}

static void write_font_dictionary(struct pdf *out_pdf)
{
}

static void write_object_offsets(struct pdf *out_pdf)
{
}

static void write_text_stream(struct pdf *out_pdf)
{
}
