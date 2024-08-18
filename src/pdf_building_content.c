#include "pdf_building.h"

const char* pdf_str_pages =
"3 0 obj\n" // %Page dictionary
"<<"
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
	">>\n"
">>\n"
"endobj\n";

const char* pdf_str_page_content =
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

static void write_page_dictionary(struct pdf* out_pdf);

static void write_page_dictionary(struct pdf* out_pdf)
{
}
