#include <stddef.h>

typedef void(*FreeFn)(void* ptr);
typedef int(*CmpFn)(void* a_ptr, void* b_ptr);

typedef struct {
  void **base;
  int *counts;
  
  int loglen;
  int aloclen;
  size_t elem_size;
  
  CmpFn cmp_fn;
  FreeFn free_fn;
  
} SortedMultiSet;

// Initialized set
void SortedMultiSetInit(SortedMultiSet* set, size_t elem_size, CmpFn cmp_fn, FreeFn free_fn);

// Frees up all allocated memory
void SortedMultiSetDestroy(SortedMultiSet* set);

// Inserts new element into set.
// Takes ownership of elem if free_fn is not NULL.
void SortedMultiSetInsert(SortedMultiSet* set, void* elem);

// Removes single instance of elem from the set.
// Does not take ownership of elem.
void SortedMultiSetRemove(SortedMultiSet* set, void* elem);

// Returns index-th element from the set.
void* SortedMultiSetGet(SortedMultiSet* set, int index);

// Returns how many times does index-th element occur in the set.
int SortedMultiSetGetCount(SortedMultiSet* set, int index);

// Returns size of the set.
int SortedMultiSetSize(SortedMultiSet* set);
