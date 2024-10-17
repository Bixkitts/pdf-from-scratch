#include "converters.h"
#include "markdown_utils.h"
#include "pdf_utils.h"

size_t convert_md_to_pdf(const struct mapped_file *md, char **out_data)
{
    struct md_all_delimiter_results parse_res = {};
    parse_markdown(md, &parse_res);

    // TODO: write the strings from the md parse results
    // using pdf_utils write functions here

    delete_md_parse_results(&parse_res);
    return 0;
}
