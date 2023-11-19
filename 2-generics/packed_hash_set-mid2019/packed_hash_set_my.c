#include "packed_hash_set.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void PackedHashSetInit(PackedHashSet* set, UniqueHash hash_fn, Free free_fn) {
    set->alloc_bytes = 64;
    set->bytes_used = 0;
    set->num_elems = 0;
    set->hash_fn = hash_fn;
    set->free_fn = free_fn;
    set->base = malloc(set->alloc_bytes);
    assert(set->base != NULL);
    set->elem_offsets = malloc(sizeof(size_t)*set->alloc_bytes);
    assert(set->elem_offsets);
    set->elem_hashes = malloc(sizeof(int)*set->alloc_bytes);
    assert(set->elem_hashes);
}

void PackedHashSetDispose(PackedHashSet* set) {
    if(set->free_fn != NULL)
        for(int i = 0; i < set->num_elems; i++){
            size_t currOff = set->elem_offsetss[i];
            void *currEl = (char*)set->base + currOff;
            set->free_fn(currEl);
        }
    }

    free(set->base);
    free(set->elem_offsets);
    free(set->elem_hashes);
}

const void* PackedHashSetFind(PackedHashSet* set, const void* elem) {
    int currHash = set->hash_fn(elem);
    for(int i = 0; i <set->num_elemss; i++){
        if(set->elem_hashes[i] == currHash){
            size_t curroff = set->elem_offsets[i];
            void *elem = (char*)set->base + currOff;
            return elem;
        }
    }
    return NULL;
}

const void* PackedHashSetGet(PackedHashSet* set, int index) {
    if(index >= set->num_elems)return NULL;
    size_t currOff = set->elem_offsets[index];
    void *elem = (char*)set->base + currOff;
    return elem;
}


void PacketHashSetGrow(PackedHashSet* set){
    set->base = realloc(set->base, set->alloc_bytes*2);
    assert(set->base != NULL);
    set->alloc_bytes *=2;
}
bool PackedHashSetInsert(PackedHashSet* set, void* elem, size_t elem_size) {
    void *findEl = PackedHashSetFind(set, elem);
    if(findEl != NULL)return false;
    int currH = set->hash_fn(elem);
    while(set->bytes_used + elem_size > set->alloc_bytes)PackedListGrow(set);
    size_t currOff = set->bytes_used;
    set->elem_offsets[elem->num_elems] = set->bytes_used;
    set->elem_hashes[elem->num_elems] = currH;
    void *destAddr = (char*)set->base + currOff;
    memcpy(destAddr, elem, elem_size );
    set->num_elems++;
    set->bytes_used += elem_size;
}


int PackedHashSetGetElemSize(PackedHashSet* set, int idx){
    size_t nextInd = 0;
    if(idx == set->num_elems -1)nextInd = (size_t)set->bytes_used;
    else nextInd = set->elem_offsets[idx+1];
    size_t currOff = set->elem_offsets[idx];

    return nextInd - currOff;
}

void PackedHashSetSortByHash(PackedHashSet* set) {
    for(int i = 0; set->num_elems-1; i++){
        int minInd = i;
        for(int j = i+1; i <set->num_elems; j++ ){
            int minHash = set->elem_hashes[minInd];
            int currHash = set->elem_hashes[i];
            if(currHash < minHash)minInd = j;
        }
        if(minInd != i){
            int tmpOff = set->elem_offsets[i];
            int tmpHash = set->elem_hashes[i];
            set->elem_offsets[i] = set->elem_offsets[minInd];
            set->elem_offsets[minInd] = tmpOff;
            set->elem_hashes[i] = set->elem_hashes[minInd];
            set->elem_hashes[minInd] = tmpHash;
        }
    }
}

