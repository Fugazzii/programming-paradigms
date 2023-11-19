#include "sorted_multi_set.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

void SortedMultiSetGrow(SortedMultiSet* set){
	set->aloclen*=2;
	set->base=realloc(set->base, sizeof(void*)*set->aloclen);
	set->counts=realloc(set->counts, sizeof(int)*set->aloclen);
}

void SortedMultiSetInit(SortedMultiSet* set, size_t elem_size, CmpFn cmp_fn, FreeFn free_fn) {
	set->loglen=0;
	set->aloclen=10;
	set->elem_size=elem_size;
	
	set->cmp_fn = cmp_fn;
	set->free_fn = free_fn;
	
	set->base=malloc(sizeof(void*)*set->aloclen);
	set->counts=malloc(sizeof(int)*set->aloclen);
}

void SortedMultiSetDestroy(SortedMultiSet* set) {
	for(int i=0; i<set->loglen; i++){
		if(set->free_fn)
			set->free_fn(SortedMultiSetGet(set, i));
		free(SortedMultiSetGet(set, i));
	}
	free(set->base);
	free(set->counts);
}

void SortedMultiSetInsert(SortedMultiSet* set, void* elem) {
	for(int i=0; i<set->loglen; i++){
		int res = set->cmp_fn(SortedMultiSetGet(set, i), elem);
		if(res==0){
			set->counts[i]++;
			if(set->free_fn)
				set->free_fn(elem);
			return;
		}
		else if(res>0){
			if(set->loglen==set->aloclen)SortedMultiSetGrow(set);
			memmove(set->base+i+1, set->base+i, (set->loglen-i)*sizeof(void*));
			memmove(set->counts+i+1, set->counts+i, (set->loglen-i)*sizeof(int));
			set->loglen++;
	
			void *tmp=malloc(set->elem_size);
			memcpy(tmp, elem, set->elem_size);
			set->base[i]=tmp;
			
			set->counts[i]=1;
			
			return;
		}
	}
	if(set->loglen==set->aloclen)SortedMultiSetGrow(set);
	
	void *tmp=malloc(set->elem_size);
	memcpy(tmp, elem, set->elem_size);
	set->base[set->loglen]=tmp;
	set->counts[set->loglen]=1;
	set->loglen++;
}

void SortedMultiSetRemove(SortedMultiSet* set, void* elem) {
	for(int i=0; i<set->loglen; i++){
		int res = set->cmp_fn(SortedMultiSetGet(set, i), elem);
		if(res==0){
			set->counts[i]--;
			if(!set->counts[i]){
				if(set->free_fn)
					set->free_fn(SortedMultiSetGet(set, i));	
				free(SortedMultiSetGet(set, i));
				memmove(set->base+i, set->base+i+1, (set->loglen-i-1)*sizeof(void*));
				memmove(set->counts+i, set->counts+i+1, (set->loglen-i-1)*sizeof(int));
				set->loglen--;
			}
		}
	}
}

void* SortedMultiSetGet(SortedMultiSet* set, int index) {
  return set->base[index];
}

int SortedMultiSetGetCount(SortedMultiSet* set, int index) {
  return set->counts[index];
}

int SortedMultiSetSize(SortedMultiSet* set) {
  return set->loglen;
}
