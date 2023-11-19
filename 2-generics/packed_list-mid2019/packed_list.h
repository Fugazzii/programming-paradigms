#ifndef PARADIGMS_PROBLEMS_PACKED_LIST_PACKED_LIST_H_
#define PARADIGMS_PROBLEMS_PACKED_LIST_PACKED_LIST_H_

#include <stddef.h>

typedef void(*Free)(void*);

typedef struct {
  void* base;
  size_t alloc_bytes;
  size_t bytes_used;
  int num_elems;
  size_t* elem_offsets;
  Free free_fn;
} PackedList;

// Initializes empty list.
void PackedListInit(PackedList* list, Free free_fn);

// Cleans up memory owned by given list.
void PackedListDispose(PackedList* list);

// Returns pointer to the index-th element in the list.
const void* PackedListGet(const PackedList* list, int index);

// Appends new element with given size in bytes to the list and
// returns pointer to it.
const void* PackedListAppend(PackedList* list, void* elem, size_t elem_size);

// Comparator function signature definition.
// Must return 0 when equal, negative number when first argument is less then
// second and positive otherwise.
typedef int(*CompareFn)(const void*, const void*);

void PackedListSort(PackedList* list, CompareFn cmp);

#endif // PARADIGMS_PROBLEMS_PACKED_LIST_PACKED_LIST_H_
