#include "sorted_list.h" 
#include <stdlib.h> 
#include <string.h> 
#include <assert.h>  
#include <stdio.h>
typedef struct{
  void *key;
  void *value;
}Elem;
void SortedListInit(SortedList *l, int key_size, int value_size,
                    CompareFn key_cmp_fn, FreeFn key_free_fn,
                    FreeFn value_free_fn) {

  // IMPLEMENT
  l->alloc_len = 4;
  l->log_len = 0;
  l->base = malloc(sizeof(Elem)*l->alloc_len);
  assert(l->base != NULL);
  l->key_size = key_size;
  l->value_size = value_size;
  l->key_cmp_fn = key_cmp_fn;
  l->key_free_fn = key_free_fn;
  l->value_free_fn = value_free_fn;
}

void SortedListDestroy(SortedList *l) {
  for(int i = 0; i < l->log_len; i++){
    Elem *currEl = (Elem*)l->base + i;
    if(l->key_free_fn != NULL)l->key_free_fn(currEl->key);
    if(l->value_free_fn != NULL)l->value_free_fn(currEl->value);
    free(currEl->key);
    free(currEl->value);
  }

  free(l->base);
}
int findElem(SortedList *l, void *key){


  for(int i =0 ; i < l->log_len; i ++){
    Elem *currEl = (Elem*)l->base + i;
    if(l->key_cmp_fn != NULL){
      if(l->key_cmp_fn(currEl->key, key) == 0)return i;
    }else{
      if(memcmp(currEl->key, key, l->key_size) == 0)return i;
    }
  }
  return -1;
}
void SortedListRealloc(SortedList *l){

  l->base = realloc(l->base, l->alloc_len*2*sizeof(Elem));
  assert(l->base != NULL);
  l->alloc_len *=2;
}
void SortedListAdd(SortedList *l, void *key, void *value) {
  int index = findElem(l, key);
  if(index == -1){
    if(l->log_len == l->alloc_len)SortedListRealloc(l);
    Elem *currEl = (Elem*)l->base + l->log_len;
    currEl->key = malloc(l->key_size);
    currEl->value = malloc(l->value_size);
    memmove(currEl->key, key, l->key_size);
    memmove(currEl->value, value, l->value_size); 
    l->log_len++;  
  }else{
    Elem *currEl = (Elem*)l->base + index;
    if(l->value_free_fn != NULL)l->value_free_fn(currEl->value);
    if(l->key_free_fn != NULL)l->key_free_fn(key);
    memmove(currEl->value, value, l->value_size);
  }
}

bool SortedListRemove(SortedList *l, void *key) {
  int index = findElem(l, key);
  if(index == -1)return false;

  Elem *currEl = (Elem*)l->base + index;
  if(l->key_free_fn != NULL)l->key_free_fn(currEl->key);
  if(l->value_free_fn != NULL)l->value_free_fn(currEl->value);
  free(currEl->key);
  free(currEl->value);
  l->log_len--;  
  if(index != l->log_len)memmove(currEl, currEl+1, sizeof(Elem)*(l->log_len-index));
  return true;
}

const void *SortedListGet(SortedList *l, void *key) {
  int index = findElem(l, key);
  if(index == -1)return NULL;
  return ((Elem*)l->base + index)->value;
}
