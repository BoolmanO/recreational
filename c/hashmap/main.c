#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>

#include "hashmap.h"
#define ITERCOUNT 469
wchar_t* arr[ITERCOUNT] = {0};

int main() 
{

  setlocale(LC_CTYPE, "en_US.UTF-8");
  HashMap* hm = hm_init(1);
  for (int i = 0; i < ITERCOUNT; i++) {
    wchar_t str[10];
    swprintf(str, 10, L"ыtr%d", i);
    arr[i] = malloc((wcslen(str) + 1)*sizeof(wchar_t*));
    wcscpy(arr[i], str);
    hm_set(hm, str, &i);
  }

  for (int i = 0; i < ITERCOUNT; i++) {
    free(arr[i]);
    arr[i] = NULL;
  }
  wprintf(L"%li %i\n", hm->size, *(int*)hm_get(hm, L"ыtr69"));
  wprintf(L"%li %i\n", hm->size, *(int*)hm_get(hm, L"ыtr420"));
  return 0;
}
