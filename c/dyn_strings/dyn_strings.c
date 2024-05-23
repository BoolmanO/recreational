#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <wctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

#include "dyn_strings.h"

#if !defined(_MSC_VER)
  FILE* printf_dummy_file = NULL;
  #define OPEN_DUMMY_FILE() (printf_dummy_file = fopen("/dev/null", "wb"))
#else
  FILE* printf_dummy_file = NULL;
  #define OPEN_DUMMY_FILE() (printf_dummy_file = fopen("NUL", "wb"))
#endif

bool __dstr__strip_cb_default(dstr_CallbackContext ctx)
{
  wchar_t c = ctx.current_wchar;
  return (c == L' ' || c == L'\t' || c == L'\n');
}

bool __dstr_true_cb_default(dstr_CallbackContext ctx __attribute__((unused)))
{
  return true;
}

dstr_CallbackContext dstr_cbctx_new(DynString source)
{
  dstr_CallbackContext ctx;
  ctx.source = source;
  return ctx;
}

void dstr_cbctx_next(dstr_CallbackContext* origin, size_t index, wchar_t current_wchar)
{
  origin->index = index;
  origin->current_wchar = current_wchar;
}

DynString dstr_init()
{
  DynString dstr;
  dstr.content = NULL;
  dstr.length = 0;
  return dstr;
}

DynString dstr_from(const wchar_t* origin)
{
  DynString dstr;
  dstr.length = wcslen(origin);
  dstr.content = malloc((dstr.length+1)*sizeof(wchar_t));
  if (dstr.content == NULL) {
    dstr.length = 0;
    return dstr;
  } 
  wcscpy(dstr.content, origin);
  return dstr;
}

DynString dstr_own(wchar_t* origin)
{
  DynString dstr;
  dstr.length = wcslen(origin);
  dstr.content = origin;
  return dstr;
}

void dstr_release(DynString* dstr)
{
  if (dstr->content != NULL) {
    free(dstr->content);
    dstr->content = NULL;
  }
  dstr->length = 0;
}

void dstr_uppercase_range_cb(DynString* dstr, size_t start, size_t end, dstr_bool_cb_t callback)
{
  if (callback == NULL) {
    callback = __dstr_true_cb_default;
  }
  dstr_CallbackContext cbctx = dstr_cbctx_new(*dstr);
  wchar_t* wstr = dstr->content;
  for (size_t i = start; wstr[i] != L'\0' && i < end; i++) {
    dstr_cbctx_next(&cbctx, i, wstr[i]);
    if (callback(cbctx)==false) break;
    wstr[i] = towupper(wstr[i]);
  }
}

void dstr_lowercase_range_cb(DynString* dstr, size_t start, size_t end, dstr_bool_cb_t callback)
{
  if (callback == NULL) {
    callback = __dstr_true_cb_default;
  }
  dstr_CallbackContext cbctx = dstr_cbctx_new(*dstr);
  wchar_t* wstr = dstr->content;
  for (size_t i = start; wstr[i] != L'\0' && i < end; i++) {
    dstr_cbctx_next(&cbctx, i, wstr[i]);
    if (callback(cbctx)==false) break;
    wstr[i] = towlower(wstr[i]);
  }
}

bool dstr_concat(DynString* origin, DynString additional)
{
  size_t len = origin->length + additional.length;
  wchar_t* new_wstr = malloc(sizeof(wchar_t) * (len+1));  // null terminator
  if (new_wstr == NULL) return false;
  wcscpy(new_wstr, origin->content);
  wcscat(new_wstr, additional.content);
  free(origin->content);
  origin->length = len; 
  origin->content = new_wstr;
  return true;
}

bool dstr_concat_sep(DynString* origin, DynString additional, DynString sep) 
{
  size_t len = origin->length + additional.length + sep.length;
  wchar_t* new_wstr = malloc(sizeof(wchar_t) * (len+1)); // null terminator
  if (new_wstr == NULL) return false;
  wcscpy(new_wstr, origin->content);
  wcscat(new_wstr, sep.content);
  wcscat(new_wstr, additional.content);
  free(origin->content);
  origin->length = len; 
  origin->content = new_wstr;
  return true;
}

bool dstr_concat_index(DynString* origin, DynString additional, size_t start_index)
{
  size_t len = origin->length + additional.length;
  wchar_t* new_wstr = malloc(sizeof(wchar_t) * (len+1)); // null terminator
  if (new_wstr == NULL) return false;
  size_t before_index_len = start_index+1; // convert index to len
  wchar_t* before_index = malloc(sizeof(wchar_t) * (before_index_len+1)); // null terminator
  if (before_index==NULL) {
    free(new_wstr);
    return false;
  }
  for (size_t i=0; i < start_index; i++) {
    before_index[i] = origin->content[i];
  }
  before_index[before_index_len] = L'\0';
 
  wcscpy(new_wstr, before_index);
  free(before_index);
  wcscat(new_wstr, additional.content);
  
  size_t after_index_len = origin->length - before_index_len + 1;
  wchar_t* after_index = malloc(sizeof(wchar_t) * (after_index_len+1)); // null terminator
  if (after_index==NULL) {
    free(new_wstr);
    return false;
  }
  for (size_t i=0; i+start_index < origin->length; i++) {
    after_index[i] = origin->content[i+start_index];
  }
  after_index[after_index_len] = L'\0';
  
  wcscat(new_wstr, after_index);
  free(after_index);
  free(origin->content);
  origin->length = len;
  origin->content = new_wstr;
  return true;
}

bool dstr_concat_index_sep(DynString* origin, DynString additional, size_t start_index, DynString sep)
{
  bool ret = dstr_concat(&sep, additional);
  if (ret==false) return false;
  return dstr_concat_index(origin, sep, start_index);
}

void dstr_strip_right_cb(DynString* dstr, dstr_bool_cb_t callback)
{
  if (dstr->content == NULL || dstr->length == 0) {
    return;
  }
  if (callback == NULL) {
    callback = __dstr__strip_cb_default;
  }

  dstr_CallbackContext cbctx = dstr_cbctx_new(*dstr);

  size_t i = dstr->length;
  while (i > 0) {
    dstr_cbctx_next(&cbctx, i, dstr->content[i-1]);
    if (callback(cbctx) == false) break;
    dstr->content[--i] = L'\0';
    dstr->length--;
  }
}

void dstr_strip_left_cb(DynString* dstr, dstr_bool_cb_t callback)
{
  if (dstr->content == NULL || dstr->length == 0) {
    return;
  }
  if (callback == NULL) {
    callback = __dstr__strip_cb_default;
  }
  dstr_CallbackContext cbctx = dstr_cbctx_new(*dstr);

  size_t start = 0;
  while (start < dstr->length) {
    dstr_cbctx_next(&cbctx, start, dstr->content[start]);
    if (callback(cbctx) == false) break;
    start++;
  }

  if (start > 0) {
    size_t new_length = dstr->length - start;
    for (size_t i = 0; i < new_length; i++) {
      dstr->content[i] = dstr->content[start + i];
    }
    dstr->content[new_length] = L'\0';
    dstr->length = new_length;
  }
}

bool dstr_format(DynString* buffer, DynString format, ...)
{
  va_list args;
  va_start(args, format);
  va_list args_copy;
  va_copy(args_copy, args);

  FILE* dummy_file = OPEN_DUMMY_FILE();
  // get the required buffer size
  size_t size = vfwprintf(dummy_file, format.content, args_copy);
  va_end(args_copy);
  fclose(dummy_file);

  buffer->content = (wchar_t*)malloc((size+1) * sizeof(wchar_t)); // null terminator
  if (buffer->content == NULL) {
    va_end(args);
    return false;
  } 
  vswprintf(buffer->content, (size+1), format.content, args);
  buffer->length = size;
  va_end(args);
  return true;
}
