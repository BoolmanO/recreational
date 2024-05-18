#include <stdbool.h>

#ifndef HASHMAP
#define HASHMAP

#ifndef HM_PAR_A
#define HM_PAR_A 690
#endif

#ifndef HM_PAR_B
#define HM_PAR_B 420
#endif

#ifndef HM_ATTEMTPS
#define HM_ATTEMTPS 5
#endif

#ifndef HM_FORMULA
#define HM_FORMULA HM_ATTEMTPS
#endif



typedef struct {
  char* key;
  char* value;
} HashMapItem;

typedef struct {
  int size;
  int count;
  HashMapItem** items;
} HashMap;


HashMap* hm_init(int size);
void* hm_get(HashMap* hm, char* key);
bool hm_ins(HashMap* hm, char* key, void* value);
void hm_del(HashMap* hm, char* key);
void hm_free(HashMap* hm);


bool hm_alloc_more(HashMap* hm, int additional_size);
#endif // !HASHMAP
