#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"
#define ITERCOUNT 469
char* arr[ITERCOUNT] = {0};

void main() 
{
  HashMap* hm = hm_init(1);
  for (int i = 0; i < ITERCOUNT; i++) {
    char str[10];
    sprintf(str, "str%d", i);
    arr[i] = malloc(strlen(str) + 1);
    strcpy(arr[i], str);
    hm_ins(hm, str, &i);
  }

  for (int i = 0; i < ITERCOUNT; i++) {
    free(arr[i]);
    arr[i] = NULL;
  }
  printf("%i %i\n", hm->size, *(hm->items[3]->value));
}
