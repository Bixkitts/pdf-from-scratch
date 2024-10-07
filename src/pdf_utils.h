#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "mem_utils.h"
#include "str_utils.h"

#define get_obj_length1(obj_strarr) \
    get_obj_length(obj_strarr, STARRLEN(obj_strarr), NULL)
#define get_obj_length2(obj_strarr, len) get_obj_length(obj_strarr, len, NULL)

struct pdf {
    char *data;
    size_t size;
    size_t capacity;
};

struct xref_section {
    const char *xref_tag;
    const char *trailer_tag;
};

struct subsection_heading {
    size_t first_obj_num;
    size_t obj_cnt;
    const char *begin;
    const char *end;
};

/*
 * PDF Reading Functions
 * -------------------------------------------
 */
size_t get_obj_length(const char *obj_strarr[], size_t len, size_t lengths[]);
char *get_with_delim(const char *str);
size_t get_stream_length(const char *obj_strarr[], int len);
const char *
find_cross_reference_section(const char *file_content, size_t content_len);
const char *find_trailer(const char *xref_tag_ptr, const char *eof);
struct xref_section
get_xref_section(const char *file_content, size_t content_len);
static const char *get_section_end(const char *ptr, size_t obj_cnt);
struct subsection_heading
get_first_section(const char *xref_tag_ptr, const char *eof);
int has_next_section(struct subsection_heading section, const char *trailer);
struct subsection_heading
get_next_section(struct subsection_heading section, const char *eof);
int has_free_objects(struct subsection_heading sec);

/*
 * PDF Writing Functions
 * --------------------------------------------
 */
void write_object_offsets(struct pdf *out_pdf);
void write_text_stream(struct pdf *out_pdf);
void write_font_dictionary(struct pdf *out_pdf);
void write_catalog(struct pdf *out_pdf);
void write_pages_catalog(struct pdf *out_pdf);
void write_page_dictionary(struct pdf *out_pdf);

/*
 * Other PDF Utilities
 * --------------------------------------------
 */
size_t obj_join(char **out, const char *obj_strarr[], size_t len);
int replace_label_in_obj(
    char *obj_strarr[],
    int len,
    const char *label,
    const char *replace);
