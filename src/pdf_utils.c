#include "pdf_utils.h"
#include <stdbool.h>

#define PDF_MINIMUM_CAPACITY 2048
#define PDF_GROWTH_RATE      2

struct pdf_obj {
    char *data;
    int elements;
    size_t size;
};

static inline int can_fit(const struct pdf *pdf, size_t size);
static void concat_pdf(struct pdf *out_pdf, const char *data, size_t size);
static void add_pdf_header(struct pdf *out_pdf);
static void add_obj_font(struct pdf *out_pdf);
static void add_obj_catalog(struct pdf *out_pdf);
static void add_obj_catalog_pages(struct pdf *out_pdf);
static void add_obj_page(struct pdf *out_pdf);
static void add_obj_metadata(struct pdf *out_pdf);
static void make_valid(struct pdf *out_pdf);
static int extend_pdf_capacity(struct pdf *out_pdf);

const char *obsraj         = "\n0000000000 65535 f";
const char *pdf_str_header = "%PDF-2.0\nTEST\n";
const char *pdf_str_metadata[] = {
    /*obj num*/" 0 obj\n"
    "<<"
    "/Type /Metadata\n"
    "/Subtype /XML\n"
    "/Length " /*num of bytes in metadata\n*/,

    ">>\n"
    "stream\n"
    "<?xpacket begin="/*Some other parameter*/,

    "id=\"W5M0MpCehiHzreSzNTczkc9d\"?>\n",
    /*Document metadata*/

    "<?xpacket end=\"w\"?>\n"
    "endstream\n"
    "endobj\n" };
const char *pdf_str_xref[] = {
    "xref\n",
    "0 "/*total number of objects then newline*/,

    /*variable: objsraj*/
    /*"BYTE_OFFSET 00000 n\n"*/
    /*"BYTE_OFFSET 00000 n\n"*/
    /*"BYTE_OFFSET 00000 n\n"*/
    /*...*/
};
const char *pdf_str_trailer =
    "trailer\n"
    "<</Size 8 /Root 1 0 R>>\n";
const char *pdf_str_xref_start =
    "startxref\n"
    /*… exact offset of the word xref from the start of the file …\n*/;
const char *pdf_str_footer = "%%EOF\n";
const char *pdf_str_font_1 =
    /*obj num*/" 0 obj\n"
    "<<"
    "/Type /Font\n"
    "/Subtype /Type1\n"
    "/BaseFont /Helvetica\n"
    "/Encoding /WinAnsiEncoding\n"
    "/FirstChar \n"
    "/LastChar \n"
    "/Widths []\n"
    "/FontDescriptor \n"
    ">>\n"
    "endobj\n";
const char *pdf_str_font_2 =
    /*obj num*/" 0 obj\n"
    "<<"
    "/Type /Font\n"
    "/Subtype /Type1\n"
    "/BaseFont /Helvetica-Oblique\n"
    "/Encoding /WinAnsiEncoding\n"
    "/FirstChar \n"
    "/LastChar \n"
    "/Widths []\n"
    "/FontDescriptor \n"
    ">>\n"
    "endobj\n";
const char *pdf_str_catalog[] = {
    /*obj num*/" 0 obj\n" // Catalog (root) object to locate everything else
    "<<"
    "/Type /Catalog\n"
    "/Pages " /*pages catalog obj number*/,

    " 0 R\n"
    "/Metadata "/*metadata obj num*/,

    " 0 R\n"
    ">>\n"
    "endobj\n"};
const char *pdf_str_catalog_pages[] = {
    /*obj num*/" 0 obj\n" // Pages catalog, containing a single page
    "<<"
    "/Type /Pages\n"
    "/Kids ["/*obj num*/,

    " 0 R]\n"
    "/Count 1\n"
    ">>\n"
    "endobj\n" };
const char *pdf_str_page[] = {
    /*obj num*/" 0 obj\n"
    "<<"
    "/Type /Page\n"
    "/Parent "/*page catalog obj num*/,

    " 0 R\n"
    "/MediaBox [0 0 612 792]\n"
    "/Contents "/*obj num*/,

    " 0 R\n"
    "/Resources "
    "<<"
    "/Font "
    "<<"
    "/F1 "/*obj num of font*/,

    " 0 R\n"
    ">>\n"
    ">>\n"
    ">>\n"
    "endobj\n" };
const char *pdf_str_page_content[] = {
    /*obj num*/" 0 obj\n" // %Page contents for page 1
    "<<"
    "/Length "/*stream length*/,

    "\n>>\n"
    "stream\n",

    /*"%This content stream writes \"Hello 32000-2 World\" and shows a number
    of\n"
    "%points of interest.\n"
    "%- There is no font styling in PDF, italic (or bold) is a change of
    font\n"
    "%- There is no automatic underlining in PDF, applications need to draw
    lines\n"
    "%- Applications will usually need to do font position calculations\n"
    "%- If extracting text, spaces are not always found in text strings,
    they\n"
    "%may come from changes to position.\n"*/
    "BT\n"
    "/F1 24 Tf\n"    // %Choose font F1, 24 points
    "72 696 Td\n"    // %Set initial x,y and set line start
    "(Hello ) Tj\n"  // %Show \"Hello \" including extra space
    "/F2 24 Tf\n"    // %Choose font F2, 24 points
    "(32000-2) Tj\n" // %Show \"32000-2\"
    "/F1 24 Tf\n"    // %Choose font F1, 24 points
    "156.1 0 Td\n"   // %Position after implied space, relative to line
    "start\n"
    "(Wor) Tj\n" // %Show \"Wor\" (implied space before is just visual)
    "(ld) Tj\n"  // %Finish word with \"ld\" (notice no space added)
    "ET\n"
    /*"%Underline \"32000-2\". Position must be calculated by generator\n"
    "%including scaled per-font underline vertical offset\n"*/
    "133.3 694.2 m\n" // %Move to start of line position
    "221.4 694.2 l\n" // %Set path to end of line position
    "1.2 w\n"         // %Set line width suitable for size of text
    "S\n",            // %Stroke path

    "endstream\n"
    "endobj\n"};

/* 
 * Hardcoded object number strings
 * TODO: replace this with flexible numbering
 */
const char *catalog_obj_num = "1";
const char *page_catalog_obj_num = "2";
const char *metadata_obj_num = "3";
const char *test_page_obj_num = "4";
const char *page_content_obj_num = "5";
const char *font_obj_num = "6";


static int extend_pdf_capacity(struct pdf *out_pdf)
{
    if (!out_pdf->capacity) {
        out_pdf->raw_data = cooler_calloc(PDF_MINIMUM_CAPACITY, sizeof(char));
        out_pdf->capacity = PDF_MINIMUM_CAPACITY;
    }
    else {
        const size_t new_size = out_pdf->capacity * PDF_GROWTH_RATE;
        out_pdf->raw_data =
            cooler_realloc(out_pdf->raw_data, new_size);
        out_pdf->capacity = new_size;
    }
    return 0;
}

size_t get_obj_length(const char *obj_strarr[], size_t len, size_t *out_lengths)
{
    size_t totalLength = 0;
    for (size_t i = 0; i < len; i++) {
        size_t clen = strlen(obj_strarr[i]);
        totalLength += clen;
        if (out_lengths)
            out_lengths[i] = clen;
    }
    return totalLength;
}

char *get_with_delim(const char *str)
{
    const char *replace_left_delim  = "{{{";
    const size_t ldlen              = sizeof("{{{") - 1;
    const char *replace_right_delim = "}}}";
    const size_t rdlen              = sizeof("}}}") - 1;
    size_t llen                     = strlen(str);
    char *c_label                   = stralloc(ldlen + rdlen + llen + 1);
    strcpy_s(c_label, ldlen + 1, replace_left_delim);
    strcpy_s(c_label + ldlen, llen + 1, str);
    strcpy_s(c_label + ldlen + llen, rdlen + 1, replace_right_delim);

    return c_label;
}

int replace_label_in_obj(
    char *obj_strarr[],
    int len,
    const char *label,
    const char *replace)
{
    char *c_label = get_with_delim(label);
    if (c_label == 0)
        return 0;
    char *out = NULL;
    int cnt   = 0;

    for (int i = 0; i < len; i++) {
        if ((out = str_replace(obj_strarr[i], c_label, strdup(replace)))) {
            obj_strarr[i] = out;
            cnt++;
        }
    }
    return cnt;
}

size_t get_stream_length(const char *obj_strarr[], int len)
{
    static const char *endstream = "\nendstream\n";
    static const char *stream    = "stream\n";
    size_t total_length          = 0;
    int index_has_stream         = -1;
    int index_has_endstream      = -1;
    char *tmp                    = NULL;
    for (int i = 0; i < len; i++) {
        const char *bit = obj_strarr[i];
        if ((tmp = strstr(bit, stream)) && index_has_stream == -1) {
            index_has_stream = i;
        }
        if ((tmp = strstr(bit, endstream)) && index_has_endstream == -1) {
            index_has_endstream = i;
        }
    }
    if (index_has_stream == -1 || index_has_endstream == -1)
        return 0;
    if (index_has_stream > index_has_endstream)
        return 0;
    if (index_has_stream == index_has_endstream)
        return 0; // TODO implement

    const char *bit = obj_strarr[index_has_stream];
    total_length +=
        count_chars_after_string(bit, stream, strlen(bit), strlen(stream));

    for (int i = index_has_stream + 1; i < index_has_endstream; i++) {
        bit            = obj_strarr[i];
        size_t count_b = count_chars_before_string(bit, endstream, strlen(bit));
        total_length += count_b;
    }
    return total_length;
}

size_t obj_join(char **out, const char *obj_strarr[], size_t len)
{
    size_t *lens   = cooler_malloc(sizeof(size_t) * len);
    size_t obj_len = get_obj_length(obj_strarr, len, lens);
    *out           = stralloc(obj_len + 1);
    for (size_t i = 0, rsum = 0; i < len; i++) {
        strcpy_s(*out + rsum, lens[i] + 1, obj_strarr[i]);
        rsum += lens[i];
    }
    return obj_len;
}

const char *
find_cross_reference_section(const char *file_content, size_t content_len)
{
    const char *res = strstr(file_content, "\nxref\n") + 1;
    if (res > file_content + content_len)
        return NULL;
    return res;
}

const char *find_trailer(const char *xref_tag_ptr, const char *eof)
{
    const char *res = strstr(xref_tag_ptr, "\ntrailer\n") + 1;
    if (res > eof)
        return NULL;
    return res;
}

struct xref_section
get_xref_section(const char *file_content, size_t content_len)
{
    struct xref_section res;
    res.xref_tag    = find_cross_reference_section(file_content, content_len);
    res.trailer_tag = find_trailer(res.xref_tag, file_content + content_len);
    return res;
}

static const char *get_section_end(const char *ptr, size_t obj_cnt)
{
    size_t i = obj_cnt;
    while (i--) {
        ptr = strstr(ptr, "\n");
    }
    return ptr;
}

struct subsection_heading
get_first_section(const char *xref_tag_ptr, const char *eof)
{
    struct subsection_heading res;
    res.begin      = xref_tag_ptr + 5;
    const char *nl = strstr(res.begin, "\n");
    const char *sp = strstr(res.begin, " ");
#ifndef SOFT_FAIL
    if (sp > nl)
        exit(1);
    if (res.begin > eof)
        exit(1);
#endif
    res.first_obj_num = strtoull_helper((char *)res.begin);
    res.obj_cnt       = strtoull_helper((char *)sp + 1);
    res.end           = get_section_end(nl, res.obj_cnt);
    return res;
}

int has_next_section(struct subsection_heading section, const char *trailer)
{
    const char *se = section.end;
    return (se + 1) == trailer;
}

struct subsection_heading
get_next_section(struct subsection_heading section, const char *eof)
{
    struct subsection_heading res;
    res.begin      = section.end + 1;
    const char *nl = strstr(res.begin, "\n");
    const char *sp = strstr(res.begin, " ");
#ifndef SOFT_FAIL
    if (sp > nl)
        exit(1);
    if (res.begin > eof)
        exit(1);
#endif
    res.first_obj_num = strtoull_helper((char *)res.begin);
    res.obj_cnt       = strtoull_helper((char *)sp + 1);
    res.end           = get_section_end(nl, res.obj_cnt);
    return res;
}

int has_free_objects(struct subsection_heading sec)
{
    char *nl = strstr(sec.begin, "\n");
    return strncmp(nl + 1, obsraj, 18) == 0;
}

static void make_valid(struct pdf *out_pdf)
{
    // 1. Find xref section
    //    if one exists, and delete it.
    //    Delete EOF stuff after it too.
    //    (What if xref is NOT at EOF...)

    // 2. Construct an xref
    //    section by parsing the pdf

    // 3. Insert it into the file,
    //    presumably at the end,
    //    after all the objects.

    // 4. After it, write EOF stuff
    //    like startxref

}

static inline int can_fit(const struct pdf *pdf, size_t size)
{
    return pdf->size + size >= pdf->capacity;
}

/*
 * We calculate the size outside of this
 * function because it could also be binary data
 */
static void concat_pdf(struct pdf *out_pdf, const char *data, size_t data_size)
{
    if (!can_fit(out_pdf, data_size)) {
        extend_pdf_capacity(out_pdf); 
    }
    memcpy(out_pdf->raw_data + out_pdf->size, data, data_size);
    out_pdf->size += data_size;
}

static void add_pdf_header(struct pdf *out_pdf)
{
    const size_t append_size = strlen(pdf_str_header);
    concat_pdf(out_pdf, pdf_str_header, append_size);
}

static void add_obj_text_stream(struct pdf *out_pdf, const char *text)
{
    out_pdf->object_count++;
}

static void add_obj_font(struct pdf *out_pdf)
{
    out_pdf->object_count++;
}

static void add_obj_metadata(struct pdf *out_pdf)
{
    out_pdf->object_count++;

}

static void add_obj_catalog(struct pdf *out_pdf)
{
    out_pdf->object_count++;
    size_t append_size = strlen(catalog_obj_num);
    concat_pdf(out_pdf, catalog_obj_num, append_size);

    append_size = strlen(pdf_str_catalog[0]);
    concat_pdf(out_pdf, pdf_str_catalog[0], append_size);

    append_size = strlen(page_catalog_obj_num);
    concat_pdf(out_pdf, page_catalog_obj_num, append_size);

    append_size = strlen(pdf_str_catalog[1]);
    concat_pdf(out_pdf, pdf_str_catalog[1], append_size);

    append_size = strlen(metadata_obj_num);
    concat_pdf(out_pdf, metadata_obj_num, append_size);

    append_size = strlen(pdf_str_catalog[2]);
    concat_pdf(out_pdf, pdf_str_catalog[2], append_size);
}

static void add_obj_catalog_pages(struct pdf *out_pdf)
{
    out_pdf->object_count++;
    size_t append_size = strlen(page_catalog_obj_num);
    concat_pdf(out_pdf, page_catalog_obj_num, append_size);

    append_size = strlen(pdf_str_catalog_pages[0]);
    concat_pdf(out_pdf, pdf_str_catalog_pages[0], append_size);

    // Here we link pages!
    append_size = strlen(test_page_obj_num);
    concat_pdf(out_pdf, test_page_obj_num, append_size);

    append_size = strlen(pdf_str_catalog_pages[1]);
    concat_pdf(out_pdf, pdf_str_catalog_pages[1], append_size);
}

static void add_obj_page(struct pdf *out_pdf)
{
    out_pdf->object_count++;
    size_t append_size = strlen(test_page_obj_num);
    concat_pdf(out_pdf, test_page_obj_num, append_size);

    append_size = strlen(pdf_str_page[0]);
    concat_pdf(out_pdf, pdf_str_page[0], append_size);

    append_size = strlen(page_catalog_obj_num);
    concat_pdf(out_pdf, page_catalog_obj_num, append_size);

    append_size = strlen(pdf_str_page[1]);
    concat_pdf(out_pdf, pdf_str_page[1], append_size);

    append_size = strlen(page_content_obj_num);
    concat_pdf(out_pdf, page_content_obj_num, append_size);

    append_size = strlen(pdf_str_page[2]);
    concat_pdf(out_pdf, pdf_str_page[2], append_size);

    append_size = strlen(font_obj_num);
    concat_pdf(out_pdf, page_content_obj_num, append_size);
}

/*
 * Create minimal objects needed to get
 * some text rendering.
 */
static void add_scaffolding(struct pdf *out_pdf)
{
    add_pdf_header(out_pdf);
    add_obj_catalog(out_pdf);
    add_obj_catalog_pages(out_pdf);
    add_obj_metadata(out_pdf);
    add_obj_page(out_pdf);
    add_obj_font(out_pdf);

}

void pdf_new(struct pdf *out_pdf)
{
    out_pdf->raw_data = cooler_calloc(PDF_MINIMUM_CAPACITY, sizeof(char));
    out_pdf->object_count = 0; // pdf object zero should not be used!
    out_pdf->capacity = PDF_MINIMUM_CAPACITY;
    out_pdf->size = 0;
}

void pdf_write_title(struct pdf *out_pdf, char *text)
{
}

void pdf_write_text(struct pdf *out_pdf, char *text)
{
}

void pdf_export(const struct pdf *pdf, const char *out_dir)
{
}

