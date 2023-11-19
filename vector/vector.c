#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <search.h>

static void VectorGrow(vector *v){
    //vector *vold = v;
    v->allocLen *= 4;
    v->elems = realloc(v->elems, v->allocLen*v->elemSize);
    assert(v->elems != NULL);
    //VectorDispose(vold);
}
void VectorNew(vector *v, int elemSize, VectorFreeFunction freeFn, int initialAllocation)
{
    assert(elemSize > 0);
    v->elemSize = elemSize;
    assert(initialAllocation >= 0);
    v->allocLen = initialAllocation;
    if(v->allocLen == 0)v->allocLen = 4;
    v->logLen = 0;
    v->elems = malloc(v->allocLen*v->elemSize);
    assert(v->elems != NULL);
    v->freefn = freeFn;
}

void VectorDispose(vector *v)
{
    if(v->freefn != NULL){
        for(int i = 0; i < v->logLen; i++){
            void *curr = (char*)v->elems + i*v->elemSize;
            v->freefn(curr);
        }
    }
    free(v->elems);
}

int VectorLength(const vector *v)
{ 
    return v->logLen;
}

void *VectorNth(const vector *v, int position)
{ 
    assert(position >= 0 && position < v->logLen);
   // fprintf(stdout, "\n can2");
    
    void *res = (char*)v->elems + position*v->elemSize;
    //fprintf(stdout, "\n can");
    return res;
}

void VectorReplace(vector *v, const void *elemAddr, int position)
{
    assert(position >= 0 && position <= v->logLen-1);
    void *pos = (char*)v->elems + position* v->elemSize;
    
    if(v->freefn != NULL)v->freefn(pos);

    memcpy(pos, elemAddr, v->elemSize);    
}

void VectorInsert(vector *v, const void *elemAddr, int position)
{
    assert(position >= 0 && position <= v->logLen);
    //space for element to insert
    if(v->logLen >= v->allocLen){
        VectorGrow(v);
    }

    void *pos = (char*)v->elems + position* v->elemSize;
    //shift memory
    memmove((char* )pos + v->elemSize, pos, v->elemSize*(v->logLen - position));
    memcpy(pos, elemAddr, v->elemSize);
    v->logLen++;
}

void VectorAppend(vector *v, const void *elemAddr)
{
    // if(v->logLen >= v->allocLen){
    //     VectorGrow(v);
    // }
    // void *target = (char*)v->elems + v->logLen*v->elemSize;
    // memcpy(target, elemAddr, v->elemSize);
    // v->logLen++;
    VectorInsert(v, elemAddr, v->logLen);
}

void VectorDelete(vector *v, int position)
{
    assert(position >=0 && position <= v->logLen-1);
    void *dest = (char*)v->elems + position*v->elemSize;
    if(v->freefn != NULL)v->freefn(dest);
    //shift memory 
    v->logLen--;
    memcpy(dest, (char*)dest + v->elemSize, v->elemSize*(v->logLen - position));
    
}
void VectorSort(vector *v, VectorCompareFunction compare)
{
    assert(compare != NULL);
    qsort(v->elems, v->logLen, v->elemSize, compare);
}

void VectorMap(vector *v, VectorMapFunction mapFn, void *auxData)
{
    assert(mapFn != NULL);
    for(int i = 0; i < v->logLen; i++){
        mapFn(VectorNth(v, i), auxData);
    }
}

static const int kNotFound = -1;
int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchFn, int startIndex, bool isSorted)
{ 
    assert(startIndex >= 0 && startIndex <= v->logLen);
    assert(key != NULL && searchFn != NULL);
    void *res = NULL;
    if(isSorted){
        res = bsearch(key, (char*)v->elems + startIndex*v->elemSize, 
                            v->logLen-startIndex, v->elemSize, searchFn);
    }else{
        size_t nelem = v->logLen-startIndex;
        res = lfind(key,(char*)v->elems+startIndex*v->elemSize,
                         &nelem, v->elemSize, searchFn );
    }
    if(res == NULL)return kNotFound; 
    else return ((char*)res - (char*)v->elems)/v->elemSize;
} 

