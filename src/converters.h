#pragma once

#include "file_handling.h"
#include <stdio.h>

#define CONVERSION_FAILED 0

size_t convert_md_to_pdf(const struct mapped_file *md, char **out_data);
