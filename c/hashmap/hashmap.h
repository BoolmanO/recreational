#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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



HashMapItem* hmi_new(wchar_t* key, void* value)
{
  HashMapItem* i = malloc(sizeof(HashMapItem));
  if (i==NULL) {
    return NULL;
  }
  i->key = wcsdup(key);
  i->value = value;
  return i;
}

void hmi_free(HashMapItem* i) 
{
  free(i->key);
  free(i->value);
  free(i);
}

unsigned long hash_function(wchar_t* str, int par)
{
  unsigned long hash = 5381;
  int c;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c + par; /* hash * 33 + c */

  return hash;
}

int get_hashed_index(wchar_t* string, int hmsize, int attempt)
{
  unsigned long hash = hash_function(string, attempt);
  return (int)((hash - attempt) % hmsize);
}

HashMap* hm_init(unsigned int size)
{
  if (size<1) {
   return NULL;
  }
  HashMap* hm = malloc(sizeof(HashMap));
  if (hm==NULL) {
    return NULL;
  }
  hm->size = size;
  hm->items = calloc((size_t)hm->size, sizeof(HashMapItem*));
  if (hm->items==NULL) {
    hm_free(hm);
    return NULL;
  }
  return hm;
}

bool hm_set(HashMap* hm, wchar_t* key, void* value)
{
  int attempt = 0;
  while (attempt < HM_FORMULA) {
    int index = get_hashed_index(key, hm->size, attempt);
    HashMapItem* item = hm->items[index];
    if (item!=NULL) {
      if (wcscmp(item->key, key) == 0) {
        free(item->value);
        item->value = value;
      }
      attempt++; 
      continue;
    }
    hm->items[index] = hmi_new(key, value);
    return true;
  }
  if(!hm_alloc_more(hm, hm->size)) {
    return false;
  }
  return hm_set(hm, key, value);
}

void* hm_get(HashMap* hm, wchar_t* key)
{
  int attempt = 0;
  while (attempt < HM_FORMULA) {
    int index = get_hashed_index(key, hm->size, attempt);
    HashMapItem* item = hm->items[index];
     
    if (item && wcscmp(item->key, key) == 0) {
      return item->value;
    }
    attempt++;
  }
  return NULL;
}

void hm_del(HashMap* hm, wchar_t* key)
{
  int attempt = 0;
  while (attempt < HM_FORMULA) {
    int index = get_hashed_index(key, hm->size, attempt);
    HashMapItem* item = hm->items[index];
    if (item!=NULL && wcscmp(item->key, key)==0) {
      hmi_free(item);
      hm->items[index] = NULL;
    }
    attempt++;
  }
}

void hm_free(HashMap* hm)
{
  if (hm->items!=NULL) {
    for (size_t i=0; i < hm->size; i++) {
      HashMapItem* item = hm->items[i];
      if (item!=NULL) {
        hmi_free(item);
      }
    }
  }
  free(hm->items);
  free(hm);
}

HashMapItem** hm_items(HashMap* hm, int* not_null)
{
  HashMapItem** arr = (HashMapItem**)calloc(hm->size, sizeof(HashMapItem*));
  if (arr==NULL) return NULL;
  *not_null = 0;
  for (size_t i=0; i < hm->size; i++) {
    HashMapItem* item = hm->items[i];
    if (item==NULL) continue;
    arr[*not_null] = item;
    (*not_null)++;
  }
  return arr;
}

bool hm_ins(HashMap* hm, HashMapItem* source)
{
  int attempt = 0;
  while (attempt < HM_FORMULA) {
    int index = get_hashed_index(source->key, hm->size, attempt);
    HashMapItem* item = hm->items[index];
    if (item!=NULL) {
      if (wcscmp(item->key, source->key) == 0) {
        hmi_free(item);
        hm->items[index] = source;
        return true;
      }
      attempt++; 
      continue;
    }
    hm->items[index] = source;
    return true;
  }
  if(!hm_alloc_more(hm, hm->size)) {
    return false;
  }
  return hm_ins(hm, source);
}

bool hm_alloc_more(HashMap* hm, size_t additional_size)
{
  if (additional_size < 1) return false;
  // overflow
  if (hm->size > SIZE_MAX / sizeof(HashMapItem*) - additional_size) return false;
  size_t new_size = hm->size+additional_size;
  HashMapItem** new_ptr = calloc(new_size, sizeof(HashMapItem*));
  if (new_ptr==NULL) return false;
  
  int not_null;
  HashMapItem** items = hm_items(hm, &not_null);

  hm->size = new_size;
  for (int i=0; i < not_null; i++) {
    HashMapItem* item = items[i];
    int attempt = 0;
    while (attempt < HM_FORMULA) {
      int index = get_hashed_index(item->key, new_size, attempt);
      if (new_ptr[index]==NULL) {
        new_ptr[index] = item;
        break;
      }
      attempt++;
    }
  }
  free(hm->items);
  hm->items = new_ptr;
  hm->size = new_size;
  free(items);
  return true;
}
// TODO: pop, hm_copy
// TODO: single header???
// TODO: {
// change HashMapItem** items to HashMapItem* items
// store bite array with NULL or not NULL
// }
// TODO: stdint
// TODO: const pointers in function signatures
#endif // !HASHMAP
