#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <search.h>

static void VectorGrow(vector *v) {
    v->allocLen *= 4;
    v->elems = realloc(v->elems, v->allocLen * v->elemSize);
}

void VectorNew(vector *v, int elemSize, VectorFreeFunction freeFn, int initialAllocation) {
    v->elemSize = elemSize;
    v->allocLen = initialAllocation;
    v->freefn = freeFn;
    v->logLen = 0;
    v->elems = malloc(v->allocLen * v->elemSize);
}

void VectorDispose(vector *v) {
    if(v->freefn == NULL) {
        return;
    }

    for(int i = 0; i < v->logLen; i++){
        void* curr = (char*)v->elems + i * v->elemSize;
        v->freefn(curr);
    }
    free(v->elems);
}

int VectorLength(const vector *v) { 
    return v->logLen;
}

void* VectorNth(const vector *v, int position) { 
    return (char*) v->elems + position * v->elemSize;
}

void VectorReplace(vector *v, const void *elemAddr, int position) {
    if(v->freefn == NULL) {
        return;
    }

    void* replaceTarget = (char*) v->elems + position * v->elemSize;
    memcpy(replaceTarget, elemAddr, v->elemSize);
}

void VectorInsert(vector *v, const void *elemAddr, int position) {    
    assert(position >= 0 && position <= v->logLen);
    if(v->logLen == v->allocLen) {
        VectorGrow(v);
    }

    memmove(
        (char*) v->elems + position * v->elemSize + v->elemSize, 
        (char*) v->elems + position * v->elemSize, 
        (v->logLen - position) * v->elemSize
    );
    memcpy((
        (char*) v->elems + position * v->elemSize,
        elemAddr,
        v->elemSize
    );
    v->logLen += 1;
}

void VectorAppend(vector *v, const void *elemAddr) {
    if(v->logLen == v->allocLen) {
        VectorGrow(v);
    }

    memcpy(
        (char*)v->elems + v->logLen * v->elemSize,
        elemAddr,
        v->elemSize
    );
    v->logLen += 1;
}

void VectorDelete(vector *v, int position) {
    assert(position >= 0 && position <= v->logLen);
    free((char*)v->elems + position * v->elemSize);
    memmove(
        (char*)v->elems + position * v->elemSize,
        (char*)v->elems + (position + 1) * v->elemSize,
        (v->logLen - position) * v->elemSize
    );
    v->logLen -= 1;
}

void VectorSort(vector *v, VectorCompareFunction compare) {
    assert(compare != NULL);
    qsort(v->elems, v->logLen, v->elemSize, compare);
}

void VectorMap(vector *v, VectorMapFunction mapFn, void* auxData) {
    assert(mapFn != NULL);
    for(int i = 0; i < v->logLen; i++){
        mapFn(VectorNth(v, i), auxData);
    }
}

static const int kNotFound = -1;

int VectorSearch(
    const vector *v,
    const void *key,
    VectorCompareFunction compareFn,
    int startIndex,
    bool isSorted
) { 
    assert(startIndex >= 0 && startIndex <= v->logLen);
    assert(key != NULL && compareFn != NULL);
    void* res = NULL;
    if(isSorted) {
        res = bsearch(
            key,
            (char*)v->elems + startIndex * v->elemSize,
            v->logLen - startIndex,
            v->elemSize,
            compareFn
        );
    }
    else {
        res = lfind(
            key,
            (char*) v->elems + startIndex * v->elemSize,
            v->logLen - startIndex, 
            v->elemSize,
            compareFn
        );
    }
    if(res == NULL)return kNotFound; 
    else return ((char*)res - (char*)v->elems) / v->elemSize;
} 