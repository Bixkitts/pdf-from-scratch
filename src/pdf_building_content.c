#include "pdf_building.h"

#include <string.h>

const char* pdf_str_pages[] = 
{ "3 0 obj\n", // %Page dictionary
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
"endobj\n" };

const char* pdf_str_page_content[] =
{ "4 0 obj\n" // %Page contents for page 1
"<<"
	"/Length {{{contentLength}}}", // TODO paste stream length here, idiot!
"\n>>\n"
	"stream\n",
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
"S\n", // %Stroke path
"endstream\n"
"endobj\n" };

static int ends_with(const char* str, const char* suffix, size_t str_len, size_t suffix_len) {
	if (!str || !suffix) return 0;
	if (suffix_len > str_len) return 0;
	return strncmp(str + str_len - suffix_len, suffix, suffix_len) == 0;
}

static int starts_with(const char* str, const char* prefix, size_t str_len, size_t prefix_len) {
	if (!str || !prefix) return 0;
	if (prefix_len > str_len) return 0;
	return strncmp(str, prefix, prefix_len) == 0;
}

static const char* ENDSTREAM = "endstream\n";
static const char* STREAM = "stream\n";

static size_t get_stream_length(const char* obj_strarr[], int len) {
	int inStream = 0;
	size_t totalLength = 0;
	for (int i = 0; i < len; i++) {
		const char* bit = obj_strarr[i];
		if (inStream) {
			if (starts_with(bit, ENDSTREAM, strlen(bit), sizeof(ENDSTREAM))) {
				inStream = 0;
			}
			else {
				totalLength += strlen(bit);
			}
		}
		else {
			if (ends_with(bit, STREAM, strlen(bit), sizeof(STREAM))) {
				inStream = 1;
			}
		}
	}
	return totalLength;
}

static void write_page_dictionary(struct pdf* out_pdf);

static void write_page_dictionary(struct pdf* out_pdf)
{
}
