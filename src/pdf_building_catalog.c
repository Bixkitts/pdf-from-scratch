#include "pdf_building.h"

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

static void write_catalog(struct pdf *out_pdf);
static void write_pages_catalog(struct pdf *out_pdf);

static void write_catalog(struct pdf *out_pdf)
{
}

static void write_pages_catalog(struct pdf *out_pdf)
{
}
