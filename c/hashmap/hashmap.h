#include <stdbool.h>
#include <wchar.h>

#ifndef HASHMAP

#define HASHMAP
#define HM_ATTEMPTS 30
#define HM_FORMULA HM_ATTEMPTS

typedef struct {
  wchar_t* key;
  void* value;
} HashMapItem;

typedef struct {
  size_t size;
  HashMapItem** items;
} HashMap;

HashMap* hm_init(unsigned int size);
void* hm_get(HashMap* hm, wchar_t* key);
HashMapItem** hm_items(HashMap* hm, int* not_null); // guarantees that items not NULL, while index in 0..not_null
bool hm_set(HashMap* hm, wchar_t* key, void* value);
bool hm_ins(HashMap* hm, HashMapItem* source);
void hm_del(HashMap* hm, wchar_t* key);
void hm_free(HashMap* hm);
bool hm_alloc_more(HashMap* hm, size_t additional_size);

#endif // !HASHMAP
