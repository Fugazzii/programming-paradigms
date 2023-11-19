#ifndef PARADIGMS_PROBLEMS_STREAMS_COMBINED_STREAM_H_
#define PARADIGMS_PROBLEMS_STREAMS_COMBINED_STREAM_H_

#include "stream.h"

// Compares values at two given pointers.
// Returns negative number if first is less than second.
// Returns positive number if first is greater than second.
// Returns zero if equal.
typedef int(*CmpFn)(void* a, void* b);

// Frees up memory occupied by object at given pointer.
typedef void(*FreeFn)(void* ptr);

typedef struct {
  FreeFn free_fn;
  CmpFn cmp_fn;
  int logSize;
  int allocSize;
  int index;
  void* elems;
} CombinedStream;

// Initializes combined stream with given streams.
// During destruction must call StreamDestroy on every stream.
void CombinedStreamInit(CombinedStream* cs,
			int num_streams,
			Stream* streams,
			CmpFn cmp_fn,
			FreeFn free_fn);

// Cleans up memory of every stream it owns.
// Must call StreamDestroy on every stream.
// Must also clean up every item retrieved from streams but not passed outside.
void CombinedStreamDestroy(CombinedStream* cs);

// Returns next smallest element among all streams.
// Returns NULL when all streams have already been consumed.
// Passes ownership of returned pointer to the caller.
void* CombinedStreamGetNext(CombinedStream* cs);

#endif // PARADIGMS_PROBLEMS_STREAMS_COMBINED_STREAM_H_
