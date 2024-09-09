#include "pdf_utils.h"

const char *obsraj         = "0000000000 65535 f";
const char *pdf_str_header = "%PDF-2.0\nPOOP\n";
const char *pdf_str_metadata =
    "7 0 obj\n" // %Document metadata
    "<<"
    "/Type /Metadata\n"
    "/Subtype /XML\n"
    "/Length … number of bytes in metadata …\n"
    ">>\n"
    "stream\n"
    "<?xpacket begin=\"… UTF-8 value of U+FEFF (efbbbf) …\" "
    "id=\"W5M0MpCehiHzreSzNTczkc9d\"?>\n"
    "… document metadata …\n"
    "<?xpacket end=\"w\"?>\n"
    "endstream\n"
    "endobj\n";
const char *pdf_str_xref =
    "xref\n"
    "0 8\n"
    "… Note that the 10 digit byte offsets here must be recalculated by the "
    "software producing the PDF …\n"
    "… and that the entries must each occupy 20 bytes including white-space …\n"
    "0000000000 65536 f\n"
    "0000000017 00000 n\n"
    "… byte offset for object 2, exactly 10 digits … 00000 n\n"
    "… byte offset for object 3, exactly 10 digits … 00000 n\n"
    "0000000848 00000 n\n"
    "0000002452 00000 n\n"
    "… byte offset for object 6, exactly 10 digits … 00000 n\n"
    "0000003309 00000 n\n";
const char *pdf_str_trailer = "trailer\n"
                              "<</Size 8 /Root 1 0 R>>\n";
const char *pdf_str_xref_start =
    "startxref\n"
    "… exact offset of the word xref from the start of the file …\n";
const char *pdf_str_footer = "%%EOF\n";
const char *pdf_str_font_1 = "5 0 obj\n" // %Font dictionary (Helvetica)
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
const char *pdf_str_font_2 = "6 0 obj\n" // %Font dictionary (Helvetica-Oblique)
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
const char *pdf_str_catalog =
    "1 0 obj\n" // Catalog (root) object to locate everything else
    "<<"
    "/Type /Catalog\n"
    "/Pages 2 0 R\n"
    "/Metadata 7 0 R\n"
    ">>\n"
    "endobj\n";
const char *pdf_str_catalog_pages =
    "2 0 obj\n" // Pages catalog, containing a single page
    "<<"
    "/Type /Pages\n"
    "/Kids [3 0 R]\n"
    "/Count 1\n"
    ">>\n"
    "endobj\n";
const char *pdf_str_pages[]        = {"3 0 obj\n", // %Page dictionary
                                      "<<",
                                      "/Type /Page\n"
                                             "/Parent 2 0 R\n"
                                             "/MediaBox [0 0 612 792]\n"
                                             "/Contents 4 0 R\n"
                                             "/Resources "
                                             "<<"
                                             "/Font "
                                             "<<"
                                             "/F1 5 0 R\n" // %Internal ref, not font name
                                      "/F2 6 0 R\n" // %Internal ref, not font name
                                      ">>\n"
                                             ">>\n",
                                      ">>\n",
                                      "endobj\n"};
const char *pdf_str_page_content[] = {
    "4 0 obj\n" // %Page contents for page 1
    "<<"
    "/Length {{{contentLength}}}", // TODO paste stream length here, idiot!
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

size_t get_obj_length(const char *obj_strarr[], size_t len, size_t lengths[])
{
    size_t totalLength = 0;
    for (size_t i = 0; i < len; i++) {
        size_t clen = strlen(obj_strarr[i]);
        totalLength += clen;
        if (lengths)
            lengths[i] = clen;
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

int replace_label_in_obj(char *obj_strarr[],
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
    static const char *ENDSTREAM = "\nendstream\n";
    static const char *STREAM    = "stream\n";
    size_t total_length          = 0;
    int index_has_stream         = -1;
    int index_has_endstream      = -1;
    char *tmp                    = NULL;
    for (int i = 0; i < len; i++) {
        const char *bit = obj_strarr[i];
        if ((tmp = strstr(bit, STREAM)) && index_has_stream == -1) {
            index_has_stream = i;
        }
        if ((tmp = strstr(bit, ENDSTREAM)) && index_has_endstream == -1) {
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
        count_chars_after_string(bit, STREAM, strlen(bit), sizeof(STREAM) - 1);

    for (int i = index_has_stream + 1; i < index_has_endstream; i++) {
        bit            = obj_strarr[i];
        size_t count_b = count_chars_before_string(bit, ENDSTREAM, strlen(bit));
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

const char *find_cross_reference_section(const char *file_content,
                                         size_t content_len)
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

struct xref_section get_xref_section(const char *file_content,
                                     size_t content_len)
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

struct subsection_heading get_first_section(const char *xref_tag_ptr,
                                            const char *eof)
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

struct subsection_heading get_next_section(struct subsection_heading section,
                                           const char *eof)
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

void write_object_offsets(struct pdf *out_pdf)
{
}

void write_text_stream(struct pdf *out_pdf)
{
}

void write_font_dictionary(struct pdf *out_pdf)
{
}

void write_catalog(struct pdf *out_pdf)
{
}

void write_pages_catalog(struct pdf *out_pdf)
{
}

void write_page_dictionary(struct pdf *out_pdf)
{
}
