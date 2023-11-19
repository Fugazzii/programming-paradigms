#include "packed_hash_set.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void PackedHashSetInit(PackedHashSet* set, UniqueHash hash_fn, Free free_fn) {
	set->hash_fn = hash_fn;
	set->free_fn = free_fn;
	set->alloc_bytes = 0;
	set->bytes_used = 0;
	set->num_elems = 0;
	
	
	set->base = NULL;
	set->elem_offsets = NULL;
	set->elem_hashes = NULL;
}

void PackedHashSetDispose(PackedHashSet* set) {
	if(set->free_fn){
		for(int i=0; i<set->num_elems; i++){
			set->free_fn((void*)PackedHashSetGet(set, i));
		}
	}
	free(set->base);
	free(set->elem_offsets);
	free(set->elem_hashes);
}

const void* PackedHashSetFind(PackedHashSet* set, const void* elem) {
	for(int i=0; i<set->num_elems; i++){
		if(set->elem_hashes[i] == set->hash_fn(elem))
			return PackedHashSetGet(set, i);
	}
	return NULL;
}

const void* PackedHashSetGet(PackedHashSet* set, int index) {
	assert(index<set->num_elems);
	return set->base + set->elem_offsets[index];
}



bool PackedHashSetInsert(PackedHashSet* set, void* elem, size_t elem_size) {
	if(PackedHashSetFind(set, elem))return false;
	set->base = realloc(set->base, set->alloc_bytes+elem_size);
	set->elem_offsets = realloc(set->elem_offsets, sizeof(size_t)*(set->num_elems+1));
	set->elem_hashes = realloc(set->elem_hashes, sizeof(int)*(set->num_elems+1));
	
	void *cur = (char*)set->base + set->bytes_used;
	memcpy(cur, elem, elem_size);
	set->elem_hashes[set->num_elems] = set->hash_fn(elem);
	set->elem_offsets[set->num_elems] = set->bytes_used;
	
	set->alloc_bytes+=elem_size;
	set->bytes_used+=elem_size;
	set->num_elems++;
	return true;
}

int PackedHashSetGetElemSize(PackedHashSet* set, int idx){
	if(idx == set->num_elems - 1)
		return set->bytes_used - set->elem_offsets[idx];
	else return set->elem_offsets[idx+1] - set->elem_offsets[idx];
}

void PackedHashSetSortByHash(PackedHashSet* set) {
	int *tmp = malloc(set->num_elems*4);
	int n = set->num_elems;
	for(int i=0; i<n; i++)
		tmp[i]=i;
	for(int i=0; i<n; i++){
		int mn = i;
		for(int j=i; j<n; j++){
			if(set->elem_hashes[tmp[j]]<set->elem_hashes[tmp[mn]])
				mn=j;
		}
		int t=tmp[i];
		tmp[i]=tmp[mn];
		tmp[mn]=t;
	}
	//void *new_base = malloc(set->alloc_bytes);
	size_t *new_offsets = malloc(sizeof(size_t)*set->num_elems);
	int *new_hashes = malloc(sizeof(int)*set->num_elems);
	
	//void *cur = new_base;
	///int cnt = 0;
	for(int i=0; i<n; i++){
		//int sz = PackedHashSetGetElemSize(set, tmp[i]);
		//memcpy(cur, PackedHashSetGet(set, tmp[i]), sz);
		//cur=(char*)cur+sz;
		
		new_offsets[i] = set->elem_offsets[tmp[i]];
		//new_offsets[i] = cnt;
		//cnt+=sz;
		
		new_hashes[i] = set->elem_hashes[tmp[i]];
	}
	//free(set->base);
	free(set->elem_offsets);
	free(set->elem_hashes);
	//set->base = new_base;
	set->elem_offsets = new_offsets;
	set->elem_hashes = new_hashes;
	free(tmp);
}

