#pragma once

#include <stdio.h>

#define CONVERSION_FAILED 0

size_t
convert_md_to_pdf(const char *in_data, size_t in_data_len, char **out_data);
