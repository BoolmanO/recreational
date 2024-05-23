#include <stdio.h>
#include <string.h>
#include <locale.h>
#include "dyn_strings.h"

int main() 
{
  setlocale(LC_ALL, "");
  DynString hello_template = dstr_from(L"             hello %ls всего тебе хорошего!             \n\n");
  DynString definition = dstr_from(L"хороший человек капибара!!      \n");
  DynString buffer = dstr_init();

  dstr_strip_both(&definition);  
  dstr_format(&buffer, hello_template, definition.content);
  dstr_uppercase_iter_cb(&buffer, 0, 6969, NULL);
  wprintf(L"%ls|end|", buffer.content);
  wprintf(L"-------------------------\n");
  dstr_strip_both_cb(&buffer, NULL);
  wprintf(L"%ls|end|", buffer.content);
  wprintf(L"\n-------------------------\n");
  dstr_uppercase_range(&buffer, 6, 9);
  wprintf(L"%ls|end|", buffer.content);

  return 0;
}
