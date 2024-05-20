#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "hashmap.h"

HashMapItem* hmi_new(char* key, void* value)
{
  HashMapItem* i = malloc(sizeof(HashMapItem));
  if (i==NULL) {
    return NULL;
  }
  i->key = strdup(key);
  i->value = strdup(value);
  return i;
}

void hmi_free(HashMapItem* i) 
{
  free(i->key);
  free(i->value);
  free(i);
}

unsigned long hash_function(char* str, int par)
{
  unsigned long hash = 5381;
  int c;

  while (c = *str++)
    hash = ((hash << 5) + hash) + c + par; /* hash * 33 + c */

  return hash;
}

int get_hashed_index(char* string, int hmsize, int attempt)
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
  hm->count = 0; // Are we really need a count?
  hm->items = calloc((size_t)hm->size, sizeof(HashMapItem*));
  if (hm->items==NULL) {
    hm_free(hm);
    return NULL;
  }
  return hm;
}

bool hm_ins(HashMap* hm, char* key, void* value)
{
  int attempt = 0;
  while (attempt < HM_FORMULA) {
    int index = get_hashed_index(key, hm->size, attempt);
    HashMapItem* item = hm->items[index];
    if (item!=NULL) {
      if (strcmp(item->key, key) == 0) {
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
  return hm_ins(hm, key, value);
}

void* hm_get(HashMap* hm, char* key)
{
  int attempt = 0;
  while (attempt < HM_FORMULA) {
    int index = get_hashed_index(key, hm->size, attempt);
    HashMapItem* item = hm->items[index];
     
    if (item && strcmp(item->key, key) == 0) {
      return item->value;
    }
    attempt++;
  }
  return NULL;
}

void hm_del(HashMap* hm, char* key)
{
  int attempt = 0;
  while (attempt < HM_FORMULA) {
    int index = get_hashed_index(key, hm->size, attempt);
    HashMapItem* item = hm->items[index];
    if (item!=NULL && strcmp(item->key, key)==0) {
      hmi_free(item);
      hm->items[index] = NULL;
    }
    attempt++;
  }
}

void hm_free(HashMap* hm)
{
  if (hm->items!=NULL) {
    for (int i=0; i < hm->size; i++) {
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
  HashMapItem** arr = (HashMapItem**)calloc((size_t)hm->size, sizeof(HashMapItem*));
  if (arr==NULL) return NULL;
  *not_null = 0;
  for (int i=0; i < hm->size; i++) {
    HashMapItem* item = hm->items[i];
    if (item==NULL) continue;
    arr[*not_null] = item;
    (*not_null)++;
  }
  return arr;
}

bool hm_alloc_more(HashMap* hm, size_t additional_size)
{
  if (additional_size < 1) return false;
  // overflow
  if (hm->size > SIZE_MAX / sizeof(HashMapItem*) - additional_size) return false; 
  size_t new_size = hm->size+additional_size;

  HashMapItem** new_ptr = realloc(hm->items, (size_t)new_size * sizeof(HashMapItem*));
  if (new_ptr==NULL) return false; 
  for (size_t i = hm->size; i < new_size; i++) {
    new_ptr[i] = NULL;
  }
  hm->items = new_ptr;
  hm->size = new_size; 
  return true;
}


// TODO: pop, copy (maybe alloc_less?)
