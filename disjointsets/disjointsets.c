#include "disjointsets.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <search.h>
#define INIT_ALLOC_N 4
#define NUM_BUCKETS 10
void DisjointSetsNew(disjointsets *ds){
    ds->dsu = malloc(sizeof(hashset)* INIT_ALLOC_N);
    assert(ds->dsu);
    ds->alloc_len = INIT_ALLOC_N;
    ds->log_len = 0;
    for(int i = 0; i < ds->alloc_len, i++){
                                                        // ???     ????
        HashSetNew(ds->dsu + i, sizeof(int), NUM_BUCKETS, hashfn, cmpfn, NULL);
    }

}
void DisjointSetsAdd(disjointsets *ds, hashset *s){
    vector *vector;
    VectorNew(v, sizeof(hashset*), NULL, ds->log_len);

    for (size_t i = 0; i < ds->log_len; i++)
    {
        
    }
    
    
}
hashset * DisjointSetsFind(disjointsets *ds, void * elemAddr){
    hashset *res = NULL;
    for(size_t i = 0; i < ds->log_len; i ++){
        void *elemSearch = HashSetLookup(ds->dsu + i, elemAddr);
        if(elemSearch != NULL){
            res = ds->dsu + i;
            break;
        }
    }
    return res;
}
void DisjointSetsDispose(disjointsets *ds){
    for(int i = 0; i < ds->alloc_len; i++){
        HashSetDispose(ds->dsu + i);
    }
    free(ds->dsu);
}