#include "tuple_hash_set.h"

void CharPairInit(CharPair* pair, char first, char second) {
	pair->tuple_type = CHAR_PAIR;
	pair->first = first;
	pair->second = second;
}

void CharTripleInit(CharTriple* triple, char first, char second, char third) {
	triple->tuple_type = CHAR_TRIPLE;
	triple->first = first;
	triple->second = second;
	triple->third = third;
}

int hashfn(const void *x){
	if(*((TupleType*)x) == CHAR_PAIR){
		CharPair *t = (CharPair*)x;
		return ((int)t->first)*256 + (int)t->second;
	}
	else {
		CharTriple *t = (CharTriple*)x;
		return ((int)t->first)*256*256 + ((int)t->second)*256 + (int)t->third+256*256*2;
	}
}

void TupleHashSetInit(TupleHashSet* set) {
	PackedHashSetInit(set, hashfn, NULL);
}

void TupleHashSetDispose(TupleHashSet* set) {
	PackedHashSetDispose(set);
}

bool TupleHashSetInsertCharPair(TupleHashSet* set, CharPair* pair) {
  return PackedHashSetInsert(set, pair, sizeof(CharPair));
}

bool TupleHashSetInsertCharTriple(TupleHashSet* set, CharTriple* triple) {
  return PackedHashSetInsert(set, triple, sizeof(CharTriple));
}

const void* TupleHashSetGet(TupleHashSet* set, int index) {
  return PackedHashSetGet(set, index);
}

const void* TupleHashSetFind(TupleHashSet* set, const void* elem) {
  return PackedHashSetFind(set, elem);
}

void TupleHashSetSort(TupleHashSet* set) {
	PackedHashSetSortByHash(set);
}
