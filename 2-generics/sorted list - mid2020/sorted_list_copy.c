#include "sorted_list.h" 
#include <stdlib.h> 
#include <string.h> 
#include <assert.h>  
typedef struct{
  void *key;
  void *value;
}Elem;
void SortedListInit(SortedList *l, int key_size, int value_size,
                    CompareFn key_cmp_fn, FreeFn key_free_fn,
                    FreeFn value_free_fn) {

  // IMPLEMENT
  l->alloc_len = 4;
  l->base = malloc(l->alloc_len * sizeof(Elem));
  assert(l->base != NULL);
  l->log_len = 0;
  l->key_size = key_size;
  l->value_size = value_size;
  l->key_cmp_fn = key_cmp_fn;
  l->key_free_fn = key_free_fn;
  l->value_free_fn = value_free_fn;
}

void SortedListDestroy(SortedList *l) {
//  if(l->key_free_fn != NULL || l->value_free_fn != NULL ){ 
      int i = 0;
      while( i< l->log_len){
          Elem *curr =(Elem*)((char*)(l->base) + i * sizeof(Elem));
          if(l->key_free_fn != NULL)l->key_free_fn((Elem*)curr->key);
          if(l->value_free_fn != NULL)l->value_free_fn((Elem*)curr->value);
          free(curr->key); free(curr->value); i++;
      }
//  }
    free(l->base);
}

void SortedListAdd(SortedList *l, void *key, void *value) {
  // IMPLEMENT
  int added = 0;
  int i = 0;
  while( i < l->log_len ){
    Elem *curr = (Elem*)l->base + i;
    if(l->key_cmp_fn(curr->key, key) == 0){ 
        if(l->value_free_fn != NULL)l->value_free_fn((Elem*)curr->value); 
        if(l->key_free_fn != NULL)l->key_free_fn(key);
        memcpy(curr->value, value, l->value_size);
        added = 1;
        break;
    }
    i ++;
  }
  if(added == 0){
    if(l->alloc_len == l->log_len){
      l->base = realloc(l->base, l->alloc_len*2 * sizeof(Elem));
      assert(l->base != NULL);
      l->alloc_len *= 2;
    }
    Elem *curr = (Elem*)l->base + l->log_len;
    curr->key = malloc(l->key_size);
    curr->value = malloc(l->value_size);
    memcpy(curr->value, value, l->value_size);
    memcpy(curr->key, key, l->key_size);

    l->log_len++;    
  }
// if(l->key_free_fn != NULL)l->key_free_fn(key);
// if(l->value_free_fn != NULL)l->value_free_fn(value);
}

bool SortedListRemove(SortedList *l, void *key) {
  int i = 0;
  while( i < l->log_len ){
    Elem *curr = (Elem*)l->base + i;
    if(l->key_cmp_fn(curr->key, key) == 0){
        if(l->key_free_fn != NULL)l->key_free_fn((Elem*)curr->key);
        if(l->value_free_fn != NULL)l->value_free_fn((Elem*)curr->value);
        free(curr->key); free(curr->value); l->log_len --;

        size_t size= (l->log_len - i) *sizeof(Elem);
        memmove(curr, curr+1, size );
       
        return true;
    }
    i ++;
  }
  return false;
}

const void *SortedListGet(SortedList *l, void *key) {
  int i = 0;
  while( i < l->log_len ){
    Elem *curr = (Elem*)l->base + i;
    if(l->key_cmp_fn(curr->key, key) == 0){
        return curr->value;
    }
    i ++;
  }
  return NULL;
}
