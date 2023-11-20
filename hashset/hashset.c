#include "hashset.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void HashSetNew(
    hashset *h, 
    int elemSize, 
    int numBuckets,
    HashSetHashFunction hashfn, 
    HashSetCompareFunction comparefn, 
    HashSetFreeFunction freefn
) {
    h->numBuckets = numBuckets;
    h->hashFn = hashfn;
    h->compareFn = comparefn;
    h->freeFn = freefn;
    h->elemSize = elemSize;
    h->size = 0;
    h->vec = malloc(sizeof(vector) * numBuckets);
    for (int i = 0; i < numBuckets; i++) {
        VectorNew(h->vec + i, elemSize, NULL, 0);
    }
}

void HashSetDispose(hashset *h) {
    for (int i = 0; i < h->numBuckets; i++) {
        VectorDispose(h->vec + i);
    }
    free(h->vec);
}

int HashSetCount(const hashset *h) { 
    return h->size;
}

void HashSetMap(hashset *h, HashSetMapFunction mapfn, void *auxData) {
    for (int i = 0; i < h->numBuckets; i++) {
        VectorMap(h->vec + i, mapfn, auxData);
    }
}

void HashSetEnter(hashset *h, const void *elemAddr) {
    int hash = h->hashFn(elemAddr, h->elemSize);
    int bucketIndex = hash % h->numBuckets;

    int foundIndex = VectorSearch(h->vec + bucketIndex, elemAddr, h->compareFn, 0, false);
    if (foundIndex == -1) {
        VectorAppend(h->vec + bucketIndex, elemAddr);
        h->size++;
    }
}

void* HashSetLookup(const hashset *h, const void *elemAddr) { 
    int hash = h->hashFn(elemAddr, h->elemSize);
    int bucketIndex = hash % h->numBuckets;

    int foundIndex = VectorSearch(h->vec + bucketIndex, elemAddr, h->compareFn, 0, false);
    if (foundIndex != -1) {
        return VectorNth(h->vec + bucketIndex, foundIndex);
    } else {
        return NULL;
    }
}
