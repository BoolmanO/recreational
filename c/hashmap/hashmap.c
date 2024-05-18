#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hashmap.h"

HashMapItem* hmi_new(char* key, void* value)
{
  HashMapItem* i = malloc(sizeof(HashMapItem));
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

unsigned long hash_function(char* string, int par, int hmsize)
{
  unsigned long hash = 0;
  int string_len = strlen(string);
  for (int i=0; i < string_len; i++) {
    hash += (unsigned long)pow(par, string_len-(i+1)) * string[i];
  }
  hash = hash % hmsize;
  return hash;
}

int get_hashed_index(char* string, int hmsize, int attempt)
{
  unsigned long hash_a = hash_function(string, HM_PAR_A, hmsize);
  unsigned long hash_b = hash_function(string, HM_PAR_B, hmsize);
  return (int)((hash_a + (attempt * (hash_b + 1)) + attempt) % hmsize);
}

HashMap* hm_init(int size)
{
  if (size<1) {
   return NULL;
  }
  HashMap* hm = malloc(sizeof(HashMap));
  hm->size = size;
  hm->count = 0; // Are we really need a count?
  hm->items = calloc((size_t)hm->size, sizeof(HashMapItem*));
  return hm;
}

bool hm_ins(HashMap* hm, char* key, void* value)
{
  int attempt = 0;
  while (attempt < HM_FORMULA) {
    int index = get_hashed_index(key, hm->size, attempt);
    if (hm->items[index]!=NULL) {
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
     
    if (item && strcmp(item->key, key)==0) {
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
  for (int i=0; i < hm->size; i++) {
    HashMapItem* item = hm->items[i];
    if (item!=NULL) {
      hmi_free(item);
    }
  }
  free(hm->items);
  free(hm);
}

bool hm_alloc_more(HashMap* hm, int additional_size) 
{
  int new_size = hm->size+additional_size;
  HashMapItem** new_ptr = realloc(hm->items, (size_t)new_size*sizeof(HashMapItem*));
  if (new_ptr==NULL) {
    return false;
  }
  for (int i = hm->size; i < new_size; i++) {
    new_ptr[i] = NULL;
  }
  hm->items = new_ptr;
  hm->size = new_size; 
  return true;
}
