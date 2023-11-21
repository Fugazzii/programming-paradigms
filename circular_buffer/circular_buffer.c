#include "circular_buffer.h"

#include <stdio.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

void CircularBufferInit(CircularBuffer* cb, int elem_size, int capacity, FreeFn free_fn, CmpFn cmp_fn) {
    cb->capacity = capacity;
    cb->elem_size = elem_size;
    cb->free_fn = free_fn;
    cb->cmp_fn = cmp_fn;
    cb->log_len = 0;
    cb->first_ind = 0;
    cb->base = malloc(cb->capacity * cb->elem_size);
}

void CircularBufferDestroy(CircularBuffer* cb) {
    assert(cb->base != NULL);
    int len = 0;
    void* curr = cb->base;
    while(len != cb->log_len) {
        curr += cb->elem_size;
        len += 1;

        if(cb->free_fn != NULL) {
            cb->free_fn(curr);
        } else {
            free(curr);
        }
    }
}

void CircularBufferPush(CircularBuffer* cb, void* value) {
    void* dest;

    if(cb->log_len == cb->capacity) {
        dest = (char*)cb->base + cb->first_ind * cb->elem_size;
        cb->first_ind = (cb->first_ind + 1) % cb->capacity; 
    } else {
        dest = (char*)cb->base + cb->log_len * cb->elem_size;
        cb->log_len += 1;
    }

    memcpy(dest, value, cb->elem_size);
}

void CircularBufferPop(CircularBuffer* cb, void* value) {
    assert(cb->log_len > 0);
    assert(cb->capacity != 0);

    if(cb->log_len < cb->capacity) {
        memcpy(value, cb->base, cb->elem_size);
        memmove(
            cb->base,
            (char*)cb->base + cb->elem_size,
            cb->elem_size * (cb->capacity - 1)
        );
    } else {
        memcpy(
            value, 
            (char*)cb->base + cb->first_ind * cb->elem_size, 
            cb->elem_size
        );
        memmove(
            (char*)cb->base + cb->first_ind * cb->elem_size,
            (char*)cb->base + (cb->first_ind + 1) % cb->capacity * cb->elem_size,
            cb->elem_size * (cb->capacity - cb->first_ind - 1)
        );

        cb->first_ind = cb->first_ind % cb->capacity;        
    }

    cb->log_len -= 1;
}

int CircularBufferCount(CircularBuffer* cb, void* value) {
    int counter = 0, len = 0;
    void* curr = cb->base;
    while(len != cb->log_len) {
        if(cb->cmp_fn(value, curr) == 0) {
            counter += 1;
        }
        curr += cb->elem_size;
        len += 1;
    }

    return counter;
}

