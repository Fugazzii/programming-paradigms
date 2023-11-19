#ifndef PARADIGMS_PROBLEMS_CIRCULAR_BUFFER_CIRCULAR_BUFFER_H_
#define PARADIGMS_PROBLEMS_CIRCULAR_BUFFER_CIRCULAR_BUFFER_H_

typedef void(*FreeFn)(void*);
typedef int(*CmpFn)(void*, void*);

typedef struct {
  void* base;
  int elem_size;
  int capacity;
  FreeFn free_fn;
  CmpFn cmp_fn;
  int first_ind;
  int log_len;
} CircularBuffer;

// Do not change

void CircularBufferInit(CircularBuffer* cb, int elem_size, int capacity, FreeFn free_fn, CmpFn cmp_fn);
void CircularBufferDestroy(CircularBuffer* cb);
void CircularBufferPush(CircularBuffer* cb, void* value);
void CircularBufferPop(CircularBuffer* cb, void* value);
int CircularBufferCount(CircularBuffer* cb, void* value);

#endif // PARADIGMS_PROBLEMS_CIRCULAR_BUFFER_CIRCULAR_BUFFER_H_
