#include "queue.h"

void QueueNew(Queue* q, size_t elem_size){
    q->base = malloc(elem_size * INIT_ALLOC_LEN);
    q->alloc_len = INIT_ALLOC_LEN;
    q->elem_size = elem_size;
    q->log_len = 0;
}

void QueueDispose(Queue* q){
    assert(q != NULL);
    free(q->base);
}

void QueueEnque(Queue* q, void* elem_addr){
    if(q->log_len == q->alloc_len) {
        q->alloc_len *= 4;
        q->base = realloc(q->base, q->alloc_len);
    }

    memcpy(
        (char*)q->base + q->log_len * q->elem_size,
        elem_addr, 
        q->elem_size
    );
    q->log_len += 1;
}

void QueueDeque(Queue * q, void * elem_addr){
    assert(q->log_len > 0);
    memcpy(elem_addr, q->base, q->elem_size);
    memmove(
        q->base,
        (char*)q->base + q->elem_size,
        (q->log_len - 1) * q->elem_size
    );    
    q->log_len -= 1;
}