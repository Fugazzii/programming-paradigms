#ifndef PARADIGMS_PROBLEMS_PACKED_HASH_SET_TUPLE_HASH_SET_H_
#define  PARADIGMS_PROBLEMS_PACKED_HASH_SET_TUPLE_HASH_SET_H_

#include "packed_hash_set.h"

typedef enum {
	      CHAR_PAIR = 0,
	      CHAR_TRIPLE = 1
} TupleType;

typedef struct {
  TupleType tuple_type;
  char first;
  char second;
} CharPair;

typedef struct {
  TupleType tuple_type;
  char first;
  char second;
  char third;
} CharTriple;

// Initialized CharPair.
void CharPairInit(CharPair* pair, char first, char second);

// Initialized CharTriple.
void CharTripleInit(CharTriple* triple, char first, char second, char third);

typedef PackedHashSet TupleHashSet;

void TupleHashSetInit(TupleHashSet* set);
void TupleHashSetDispose(TupleHashSet* set);
bool TupleHashSetInsertCharPair(TupleHashSet* set, CharPair* pair);
bool TupleHashSetInsertCharTriple(TupleHashSet* set, CharTriple* triple);
const void* TupleHashSetGet(TupleHashSet* set, int index);
const void* TupleHashSetFind(TupleHashSet* set, const void* elem);
void TupleHashSetSort(TupleHashSet* set);

#endif // PARADIGMS_PROBLEMS_PACKED_HASH_SET_TUPLE_HASH_SET_H_
