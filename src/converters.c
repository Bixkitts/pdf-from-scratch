#include "converters.h"
#include "markdown_utils.h"
#include "pdf_utils.h"

size_t convert_md_to_pdf(const struct mapped_file *md, char **out_data)
{
    struct md_all_delimiter_results parse_res = {};
    parse_markdown(md, &parse_res);

    struct pdf out_pdf = {};
    // Order of stuff I need to write:
    // 0.  "%PDF-2.0"
    //
    // Objects:
    // 1.  Catalog
    // 2.  Pages
    // 3.  Page
    // 4.  Content Stream
    // 5.  Font
    // 6.  Another Font...
    // 7.  Metadata Stream
    //
    // 8.  Xref (list of object offsets)
    // 9.  "startxref\n" (exact offset of "xref" from start of file)
    // 10. %%EOF
    //
    // TODO: each of these objects should be
    // properly inserted into the PDF
    // in any order
    pdf_write_title(&out_pdf, "Test");
    pdf_write_text(&out_pdf, "lorem ipsum\n");

    delete_md_parse_results(&parse_res);
    return 0;
}
