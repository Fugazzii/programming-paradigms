#ifndef _dsu_
#define _dsu_

#include "hashset.h"
#include "vector.h"
//assume that this function is already implemented
void SetUnion(hashset *a, const hashset *b);

typedef struct disjointsets{
    hashset *dsu;
    size_t alloc_len;
    size_t log_len;
};
void DisjointSetsNew(disjointsets *ds);
void DisjointSetsAdd(disjointsets *ds, hashset *s);
hashset * DisjointSetsFind(disjointsets *ds, void * elemAddr);
void DisjointSetsDispose(disjointsets *ds);

#endif