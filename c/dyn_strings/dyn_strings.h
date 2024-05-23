#ifndef DYN_STRINGS
#define DYN_STRINGS
#include <wchar.h>
#include <stdbool.h>

typedef struct {
  wchar_t* content;
  size_t length;
} DynString;

typedef struct {
  DynString source;
  size_t index;
  wchar_t current_wchar;
} dstr_CallbackContext;

typedef bool (*dstr_bool_cb_t)(dstr_CallbackContext);
typedef wchar_t (*dstr_wchar_cb_t)(dstr_CallbackContext);

// just initialzie struct with NULL content and 0 length
// use this only for creating buffers
DynString dstr_init();
// content can be null, if you use dstr_from
// you need to check it yourself.
DynString dstr_from(const wchar_t* origin);
// dstr_own just use pointer that given
// when dstr_from copy the data to another pointer
// keep in mind string should be dynamically allocated.
// or don't call dstr_release.
DynString dstr_own(wchar_t* origin);
// just free the content and set size to 0
void dstr_release(DynString *dstr);

void dstr_uppercase_cb(DynString* dstr, dstr_bool_cb_t callback);
void dstr_uppercase_range_cb(DynString* dstr, size_t start, size_t end, dstr_bool_cb_t callback);
void dstr_uppercase_iter_cb(DynString* dstr, size_t start, size_t iterations, dstr_bool_cb_t callback);
void dstr_lowercase_cb(DynString* dstr, dstr_bool_cb_t callback);
void dstr_lowercase_range_cb(DynString* dstr, size_t start, size_t end, dstr_bool_cb_t callback);
void dstr_lowercase_iter_cb(DynString* dstr, size_t start, size_t iterations, dstr_bool_cb_t callback);

void dstr_uppercase(DynString* dstr);
void dstr_uppercase_range(DynString* dstr, size_t start, size_t end);
void dstr_uppercase_iter(DynString* dstr, size_t start, size_t iterations);
void dstr_lowercase(DynString* dstr);
void dstr_lowercase_range(DynString* dstr, size_t start, size_t end);
void dstr_lowercase_iter(DynString* dstr, size_t start, size_t iterations);

bool dstr_concat(DynString* origin, DynString additional);
bool dstr_concat_sep(DynString* origin, DynString additional, DynString sep);
bool dstr_concat_index(DynString* origin, DynString additional, size_t start_index);
bool dstr_concat_index_sep(DynString* origin, DynString additional, size_t start_index, DynString sep);

// if in _strip function callback is NULL, using default strip that look for '\n' '\t' and ' ' 
void dstr_strip_right_cb(DynString* dstr, dstr_bool_cb_t callback);
void dstr_strip_left_cb(DynString* dstr, dstr_bool_cb_t callback);
void dstr_strip_both_cb(DynString* dstr, dstr_bool_cb_t callback);

// just shorthands for dstr_strip_*_cb with NULL callback (using default strip);
void dstr_strip_right(DynString* dstr);
void dstr_strip_left(DynString* dstr);
void dstr_strip_both(DynString* dstr);

bool dstr_format(DynString* buffer, DynString format, ...);

#define dstr_uppercase_cb(dstr, callback) \
  dstr_uppercase_range_cb((dstr), 0, (dstr->length-1), (callback))

#define dstr_uppercase_iter_cb(dstr, start, iterations, callback) \
    dstr_uppercase_range_cb((dstr), (start), (start) + (iterations), (callback))

#define dstr_lowercase_cb(dstr, callback) \
  dstr_lowercase_range_cb((dstr), 0, (dstr->length-1), (callback))

#define dstr_lowercase_iter_cb(dstr, start, iterations, callback) \
  dstr_lowercase_range_cb((dstr), (start), (start) + (iterations), (callback))

#define dstr_uppercase(dstr) \
  dstr_uppercase_range_cb((dstr), 0, (dstr->length-1), NULL)

#define dstr_uppercase_range(dstr, start, end) \
  dstr_uppercase_range_cb((dstr), (start), (end), NULL)

#define dstr_uppercase_iter(dstr, start, iterations) \
  dstr_uppercase_range_cb((dstr), (start), (start) + (iterations), NULL)

#define dstr_lowercase(dstr) \
  dstr_lowercase_range_cb((dstr), 0, (dstr->length-1), NULL)

#define dstr_lowercase_range(dstr, start, end) \
  dstr_lowercase_range_cb((dstr), (start), (end), NULL)

#define dstr_lowercase_iter(dstr, start, iterations) \
  dstr_lowercase_range_cb((dstr), (start), (start) + (iterations), NULL)


#define dstr_strip_both_cb(dstr, callback) \
  do { \
    dstr_strip_right_cb((dstr), (callback)); \
    dstr_strip_left_cb((dstr), (callback)); \
  } while (0)

#define dstr_strip_left(dstr) \
  dstr_strip_left_cb((dstr), NULL)

#define dstr_strip_right(dstr) \
  dstr_strip_right_cb((dstr), NULL)

#define dstr_strip_both(dstr) \
  dstr_strip_both_cb((dstr), NULL)

// TODO:
// bool dstr_remove(DynString* origin, size_t index);
// bool dstr_remove_range(DynString* origin, size_t start, size_t end);
// bool dstr_remove_iter(DynString* origin, size_t start, size_t iterations);

// TODO: single header library

// TODO:
// void dstr_separate(DynString* origin, DynString* buffer, index);
// void dstr_separate_iter(DynString* origin, ...) ???

#endif // !DYN_STRINGS
