#include <stdbool.h>

#ifndef HASHMAP

#define HASHMAP
#define HM_ATTEMTPS 30
#define HM_FORMULA HM_ATTEMTPS

typedef struct {
  char* key;
  char* value;
} HashMapItem;

typedef struct {
  size_t size;
  int count;
  HashMapItem** items;
} HashMap;

HashMap* hm_init(unsigned int size);
void* hm_get(HashMap* hm, char* key);
bool hm_ins(HashMap* hm, char* key, void* value);
void hm_del(HashMap* hm, char* key);
void hm_free(HashMap* hm);
HashMapItem** hm_items(HashMap* hm, int* not_null);
bool hm_alloc_more(HashMap* hm, int additional_size);

#endif // !HASHMAP
