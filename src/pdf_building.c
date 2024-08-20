#include "pdf_building.h"

// Strings we write into the PDF file.
// We _could_ replace this with something
// other than hardcoded strings in the binary.
const char *pdf_str_header  =
"%PDF-2.0\nPOOP\n";

const char *pdf_str_metadata =
"7 0 obj\n" // %Document metadata
"<<"
    "/Type /Metadata\n"
    "/Subtype /XML\n"
    "/Length … number of bytes in metadata …\n"
">>\n"
"stream\n"
"<?xpacket begin=\"… UTF-8 value of U+FEFF (efbbbf) …\" id=\"W5M0MpCehiHzreSzNTczkc9d\"?>\n"
"… document metadata …\n"
"<?xpacket end=\"w\"?>\n"
"endstream\n"
"endobj\n";

const char *pdf_str_xref =
"xref\n"
"0 8\n"
"… Note that the 10 digit byte offsets here must be recalculated by the software producing the PDF …\n"
"… and that the entries must each occupy 20 bytes including white-space …\n"
"0000000000 65536 f\n"
"0000000017 00000 n\n"
"… byte offset for object 2, exactly 10 digits … 00000 n\n"
"… byte offset for object 3, exactly 10 digits … 00000 n\n"
"0000000848 00000 n\n"
"0000002452 00000 n\n"
"… byte offset for object 6, exactly 10 digits … 00000 n\n"
"0000003309 00000 n\n";

const char *pdf_str_trailer =
"trailer\n"
"<</Size 8 /Root 1 0 R>>\n";

const char *pdf_str_xref_start =
"startxref\n"
"… exact offset of the word xref from the start of the file …\n";

const char *pdf_str_footer =
"%%EOF\n";

// Pdf creation helpers.
// Each one writes a different object
// to the pdf.
static void write_object_offsets  (struct pdf *out_pdf);
static void write_text_stream     (struct pdf *out_pdf);

// return the size of the pdf and -1 on error
size_t build_pdf_from_md(const char *md,
                      size_t md_size,
                      struct pdf *out_pdf)
{
    size_t pdf_capacity = md_size * 2;
    // need size of md + size of pdf metadata
    out_pdf->data = calloc(pdf_capacity, 1);

    // Call the pdf creation helpers here

    return out_pdf->len;
}

static void write_object_offsets(struct pdf *out_pdf)
{
}

static void write_text_stream(struct pdf *out_pdf)
{
}
