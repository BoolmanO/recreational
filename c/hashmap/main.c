#include <stdio.h>
#include <unistd.h>
#include "hashmap.h"

void main() 
{
  HashMap* hm = hm_init(1);
  int my_integer = 69;
  int my_integer_second = 420;
  int my_integer_third = 69420;
  hm_ins(hm, "imposter", &my_integer);
  hm_ins(hm, "sus", &my_integer_second);
  hm_ins(hm, "amogus", &my_integer_third);
  
  hm_del(hm, "imposter");
  
  printf("%i %i %i\n", 
    hm_get(hm, "imposter")!=NULL, // should be 0
    hm_get(hm, "sus")!=NULL, // should be 1
    hm_get(hm, "amogus")!=NULL // should be 1
  );

  printf("%i\n", hm->size);
}
