#include "vector.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void VectorInit(Vector* v, int elem_size, FreeFn free_fn) {
  v->elem_size = elem_size;
  v->capacity = 4;
  v->log_len = 0;
  v->free_fn = free_fn;
  v->base = malloc(v->capacity * v->elem_size);
}

void VectorDestroy(Vector* v) {
  for(int i = 0; i < v->log_len; i++) {
    if(v->free_fn != NULL) {
      v->free_fn((char*) v->base + i * v->elem_size);
    }
  }
  free(v->base);
}

void VectorAppend(Vector* v, void* elem) {
  if(v->log_len == v->capacity) {
    v->capacity *= 2;
    v->base = realloc(v->base, v->capacity*v->elem_size);
  }

  memcpy(
    (char*)v->base + v->log_len * v->elem_size,
    elem,
    v->elem_size
  );

  v->log_len += 1;
}

void VectorInsert(Vector* v, int index, void* elem) {
  if(v->log_len == v->capacity) {
    v->capacity *= 2;
    v->base = realloc(v->base, v->capacity*v->elem_size);
  }
  
  // 0 1 2 3
  memmove(
    (char*) v->base + (index + 1) * v->elem_size,
    (char*) v->base + index * v->elem_size,
    (v->log_len - index) * v->elem_size
  );

  memcpy(
    (char*)v->base + index * v->elem_size,
    elem,
    v->elem_size
  );
  v->log_len += 1;
}

void VectorOverwrite(Vector* v, int index, void* elem) {
  assert(index < v->log_len);
  memcpy(
    (char*)v->base + index * v->elem_size,
    elem,
    v->elem_size
  );
}

void* VectorGet(Vector* v, int index) {
  assert(v->log_len > index);
  assert(v->base != NULL);
  void* result = (char*)v->base + index * v->elem_size;
  return result;
}

void VectorRemove(Vector* v, int index, void* elem) {
  assert(v->log_len > 0);

  // Write target data into elem
  if(elem != NULL) {
    memcpy(
      elem,
      (char*)v->base + index * v->elem_size,
      v->elem_size
    );
  }
  // Reduce all data index by one
  memmove(
    (char*)v->base + index * v->elem_size,
    (char*)v->base + (index + 1) * v->elem_size,
    (v->log_len - index) * v->elem_size
  );

  v->log_len -= 1;
}

int VectorSize(Vector* v) {
  return v->log_len;
}

