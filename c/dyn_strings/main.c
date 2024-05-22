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

  dstr_strip_both(&definition); // equal to dstr_strip_both_cb(&definition, NULL)
  
  dstr_format(&buffer, hello_template, definition.content);
  
  wprintf(L"%ls", buffer.content);
  wprintf(L"-------------------------\n");
  dstr_strip_both_cb(&buffer, NULL); // equal to dstr_strip_both(&buffer)
  wprintf(L"%ls", buffer.content);
  wprintf(L"\n-------------------------\n"); // default strip also remove \n (u can pass ur own strip callback)
  dstr_uppercase_range(&buffer, 6, 9);
  wprintf(L"%ls", buffer.content);

  return 0;
}
