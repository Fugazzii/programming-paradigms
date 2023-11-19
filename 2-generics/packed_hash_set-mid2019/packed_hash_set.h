#ifndef PARADIGMS_PROBLEMS_PACKED_HASH_SET_PACKED_HASH_SET_H_
#define PARADIGMS_PROBLEMS_PACKED_HASH_SET_PACKED_HASH_SET_H_

#include <stdbool.h>
#include <stddef.h>

typedef void(*Free)(void*);

typedef int(*UniqueHash)(const void*);

typedef struct {
  void* base;
  size_t alloc_bytes;
  size_t bytes_used;
  int num_elems;
  size_t* elem_offsets;
  int* elem_hashes;
  UniqueHash hash_fn;
  Free free_fn;
} PackedHashSet;

// Initializes empty set.
void PackedHashSetInit(PackedHashSet* set, UniqueHash hash_fn, Free free_fn);

// Cleans up memory owned by given set.
void PackedHashSetDispose(PackedHashSet* set);

// If given data exists in the set returns pointer to it's corresponding
// element otherwise NULL.
const void* PackedHashSetFind(PackedHashSet* set, const void* elem);

const void* PackedHashSetGet(PackedHashSet* set, int index);

// Inserts new element with given size in bytes into the set.
// Returns false if element with same hash value already existed in the set
// and true otherwise.
bool PackedHashSetInsert(PackedHashSet* set, void* elem,
			 size_t elem_size);

void PackedHashSetSortByHash(PackedHashSet* set);

#endif // PARADIGMS_PROBLEMS_PACKED_HASH_SET_PACKED_HASH_SET_H_
