#include "pdf_building.h"

const char* pdf_str_font_1 =
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

const char* pdf_str_font_2 =
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

static void write_font_dictionary(struct pdf* out_pdf);

static void write_font_dictionary(struct pdf* out_pdf)
{
}
