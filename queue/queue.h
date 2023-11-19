#ifndef _queue_
#define _queue_

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define INIT_ALLOC_LEN 4

typedef struct 
{
    size_t elem_size;
    size_t alloc_len;
    size_t log_len;
    void *base;
}Queue;
void QueueNew(Queue * q, size_t elemSize);
void QueueDispose(Queue * q);
void QueueEnque(Queue * q, void * elemAddr);
void QueueDeque(Queue * q, void * elemAddr);

#endif