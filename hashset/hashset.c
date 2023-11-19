#include "hashset.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
void HashSetNew(hashset *h, int elemSize, int numBuckets,
		HashSetHashFunction hashfn, HashSetCompareFunction comparefn, HashSetFreeFunction freefn)
{
	//fprintf(stdout, "\nNew1");

	assert(numBuckets > 0 && elemSize > 0 );
	assert(comparefn != NULL && hashfn != NULL);

	h->numBuckets = numBuckets;
	h->hashTable = malloc(sizeof(vector)*h->numBuckets);
	assert(h->hashTable != NULL);
	h->hashFn = hashfn;
	h->compareFn = comparefn;
	h->freeFn = freefn;
	h->size = 0;
	h->elemSize = elemSize;
	for(int i = 0; i < h->numBuckets; i++){
		vector *v = (h->hashTable + i);
		VectorNew(v, h->elemSize, h->freeFn, 1);
	}
	//fprintf(stdout, "\nNew2");
}

void HashSetDispose(hashset *h)
{
	//fprintf(stdout, "\ndisp1");

	for(int i = 0 ; i < h->numBuckets; i++){
		VectorDispose(h->hashTable +i );
		//free(h->hashTable[i]); 
	}
	free(h->hashTable);
	//fprintf(stdout, "\ndisp2");

}

int HashSetCount(const hashset *h)
{ 
	//fprintf(stdout, "\ncount");

	return h->size; 
}

void HashSetMap(hashset *h, HashSetMapFunction mapfn, void *auxData)
{
	assert(mapfn != NULL);
	//fprintf(stdout, "\nmap1");

	for(int i = 0; i < h->numBuckets; i++){
		VectorMap(&(h->hashTable[i]), mapfn, auxData);
	}
	//fprintf(stdout, "\nmap2");

}

void HashSetEnter(hashset *h, const void *elemAddr)
{
	assert(elemAddr != NULL);
	int buckInd = h->hashFn(elemAddr, h->numBuckets);

	assert(buckInd >= 0 && buckInd < h->numBuckets);
	vector *currVec = (h->hashTable+buckInd);
	//look for element in the current vector:
	//if(currVec->logLen == 0)return;
	int pos = VectorSearch(currVec, elemAddr, h->compareFn, 0, true);
	//not found -> append to vector
	if(pos == -1){
		h->size++;
		VectorAppend(currVec, elemAddr);
		VectorSort(currVec, h->compareFn);
	}else{
		//found-> replace
		VectorReplace(currVec, elemAddr, pos);
	}
}

void *HashSetLookup(const hashset *h, const void *elemAddr)
{ 

	assert(elemAddr != NULL);
	int buckInd = h->hashFn(elemAddr, h->numBuckets);

	assert(buckInd >= 0 && buckInd < h->numBuckets);
	vector *currVec = (h->hashTable+buckInd);

	//look for elem in addr
	int pos = VectorSearch(currVec, elemAddr, h->compareFn, 0, true);
	
	if(pos == -1){
		return NULL; 
	}
	else{
		return VectorNth(currVec, pos);
	}

}
