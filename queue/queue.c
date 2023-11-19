#include "queue.h"

void QueueNew(Queue * q, size_t elemSize){
    assert(elemSize > 0);
    q->base = malloc(INIT_ALLOC_LEN);
    assert(q->base != NULL);
    q->alloc_len = INIT_ALLOC_LEN;
    q->elem_size = elemSize;
    q->log_len = 0;

}
void QueueDispose(Queue * q){
    free(q->base);
}
void QueueEnque(Queue * q, void * elemAddr){
    if(q->log_len == q->alloc_len){
        q->alloc_len *=2;
        realloc(q->base, q->alloc_len);
        assert(q->base != NULL);
    }
    memcpy((char *)q->base + q->elem_size*q->log_len, elemAddr, q->elem_size);

}
void QueueDeque(Queue * q, void * elemAddr){
    assert(q->log_len > 0);
    q->log_len--;
    memcpy(elemAddr, q->base, q->elem_size);
    memmove(q->base, (char*)q->base + q->elem_size, q->elem_size*q->log_len);

}