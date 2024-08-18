#include "pdf_building.h"

// Strings we write into the PDF file.
// We _could_ replace this with something
// other than hardcoded strings in the binary.
const char *pdf_str_header  =
"%PDF-2.0\nPOOP\n";

const char *pdf_str_catalog =
"1 0 obj\n" //Catalog (root) object to locate everything else
"<<"
    "/Type /Catalog\n"
    "/Pages 2 0 R\n"
    "/Metadata 7 0 R\n"
">>\n"
"endobj\n";

const char *pdf_str_catalog_pages =
"2 0 obj\n" //Pages catalog, containing a single page
"<<"
    "/Type /Pages\n"
    "/Kids [3 0 R]\n"
    "/Count 1\n"
">>\n"
"endobj\n";

const char *pdf_str_pages =
"3 0 obj\n" // %Page dictionary
"<<"
    "/Type /Page\n"
    "/Parent 2 0 R\n"
    "/MediaBox [0 0 612 792]\n"
    "/Contents 4 0 R\n"
    "/Resources <<"
        "/Font <<"
            "/F1 5 0 R\n" // %Internal ref, not font name
            "/F2 6 0 R\n" // %Internal ref, not font name
        ">>\n"
    ">>\n"
">>\n"
"endobj\n";

const char *pdf_str_page_content =
"4 0 obj\n" // %Page contents for page 1
"<<"
    "/Length … number of bytes in page marking operators …\n"
">>\n"
"stream\n"
        /*"%This content stream writes \"Hello 32000-2 World\" and shows a number of\n"
        "%points of interest.\n"
        "%- There is no font styling in PDF, italic (or bold) is a change of font\n"
        "%- There is no automatic underlining in PDF, applications need to draw lines\n"
        "%- Applications will usually need to do font position calculations\n"
        "%- If extracting text, spaces are not always found in text strings, they\n"
        "%may come from changes to position.\n"*/
"BT\n"
    "/F1 24 Tf\n" // %Choose font F1, 24 points
    "72 696 Td\n" // %Set initial x,y and set line start
    "(Hello ) Tj\n" // %Show \"Hello \" including extra space
    "/F2 24 Tf\n" // %Choose font F2, 24 points
    "(32000-2) Tj\n" // %Show \"32000-2\"
    "/F1 24 Tf\n" // %Choose font F1, 24 points
    "156.1 0 Td\n" // %Position after implied space, relative to line
    "start\n"
    "(Wor) Tj\n" // %Show \"Wor\" (implied space before is just visual)
    "(ld) Tj\n" // %Finish word with \"ld\" (notice no space added)
"ET\n"
/*"%Underline \"32000-2\". Position must be calculated by generator\n"
"%including scaled per-font underline vertical offset\n"*/
"133.3 694.2 m\n" // %Move to start of line position
"221.4 694.2 l\n" // %Set path to end of line position
"1.2 w\n" // %Set line width suitable for size of text
"S\n" // %Stroke path
"endstream\n"
"endobj\n";

const char *pdf_str_font_1 =
"5 0 obj\n" // %Font dictionary (Helvetica)
"<<"
    "/Type /Font\n"
    "/Subtype /Type1\n"
    "/BaseFont /Helvetica\n"
    "/Encoding /WinAnsiEncoding\n"
    "/FirstChar …\n"
    "/LastChar …\n"
    "/Widths […]\n"
    "/FontDescriptor …\n"
">>\n"
"endobj\n";

const char *pdf_str_font_2 =
"6 0 obj\n" // %Font dictionary (Helvetica-Oblique)
"<<"
    "/Type /Font\n"
    "/Subtype /Type1\n"
    "/BaseFont /Helvetica-Oblique\n"
    "/Encoding /WinAnsiEncoding\n"
    "/FirstChar …\n"
    "/LastChar …\n"
    "/Widths […]\n"
    "/FontDescriptor …\n"
">>\n"
"endobj\n";

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