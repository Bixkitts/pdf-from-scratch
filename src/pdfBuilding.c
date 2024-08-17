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

struct Pdf *build_pdf_from_md(const char *md, size_t size)
{
    // Call the pdf creation helpers here
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
