#include "packed_list.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
void PackedListInit(PackedList* list, Free free_fn) {
    list->alloc_bytes = 64;
    list->bytes_used = 0;
    list->num_elems = 0;
    list->free_fn = free_fn;

    list->base = malloc(list->alloc_bytes);
    assert(list->base != NULL);
    list->elem_offsets = malloc(list->alloc_bytes * sizeof(size_t));
    assert(list->elem_offsets != NULL);
}

void PackedListDispose(PackedList* list) {
    if(list->free_fn != NULL){
        for(int i = 0; i < list->num_elems; i ++){
            size_t offs = list->elem_offsets[i];
            void *currElem = (char*)list->base + offs;
            list->free_fn(currElem);
        }
    }
    free(list->base);
    free(list->elem_offsets);

}

const void* PackedListGet(const PackedList* list, int index) {
    size_t offs = list->elem_offsets[index];
    void *currElem = (char*)list->base + offs;
    return currElem;
}
void PackedListGrow(PackedList* list){
    list->base = realloc(list->base, list->alloc_bytes *2);
    assert(list->base != NULL);
    list->elem_offsets = realloc(list->elem_offsets, list->alloc_bytes*2*sizeof(size_t));
    assert(list->elem_offsets != NULL);
    list->alloc_bytes *= 2;

}
const void* PackedListAppend(PackedList* list, void* elem, size_t elem_size) {
    while(list->bytes_used+elem_size > list->alloc_bytes)PackedListGrow(list);
    list->elem_offsets[list->num_elems] = list->bytes_used; //where ends last elem
    void *destAddr = (char*)list->base + list->elem_offsets[list->num_elems];
    memcpy(destAddr, elem, elem_size);        
    list->bytes_used+= elem_size;    
    list->num_elems++;
  return NULL;
}

void PackedListSort(PackedList* list, CompareFn cmp) {
    for(int i = 0; i < list->num_elems-1; i++){
        int minindex = i;
        for(int j = i+1; j< list->num_elems; j++){
            size_t currOff = list->elem_offsets[j];
            size_t minOff = list->elem_offsets[minindex];
            void *elemMin = (char *)list->base + minOff;
            void *elemCmp = (char *)list->base + currOff;
            if(cmp(elemCmp, elemMin) < 0)minindex = j;
        }
        if(minindex != i){
            size_t tmp = list->elem_offsets[i];
            list->elem_offsets[i] = list->elem_offsets[minindex];
            list->elem_offsets[minindex] = tmp;

        }
    }
}
