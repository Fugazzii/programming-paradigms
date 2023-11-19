#include "packed_hash_set.h"
#include "tuple_hash_set.h"

#include <string.h>

#include "libs_DO_NOT_TOUCH/lunit.h"

// PackedHashSet integers

int HashInt(const void* pt) {
  return *(int*)pt;
}

TEST(Ints_Empty) {
  PackedHashSet set;
  PackedHashSetInit(&set, HashInt, /*free_fn=*/NULL);
  ASSERT_INT_EQ(set.num_elems, 0);
  PackedHashSetDispose(&set);
}

TEST(Ints_Basic) {
  PackedHashSet set;
  PackedHashSetInit(&set, HashInt, /*free_fn=*/NULL);
  ASSERT_INT_EQ(set.num_elems, 0);
  int one = 1, two = 2, three = 3;
  ASSERT_TRUE(PackedHashSetInsert(&set, &one, sizeof(int)));
  ASSERT_INT_EQ(set.num_elems, 1);
  ASSERT_TRUE(PackedHashSetInsert(&set, &two, sizeof(int)));
  ASSERT_INT_EQ(set.num_elems, 2);
  ASSERT_TRUE(PackedHashSetInsert(&set, &three, sizeof(int)));
  ASSERT_INT_EQ(set.num_elems, 3);
  PackedHashSetDispose(&set);
}

TEST(Ints_Sort) {
  PackedHashSet set;
  PackedHashSetInit(&set, HashInt, /*free_fn=*/NULL);
  ASSERT_INT_EQ(set.num_elems, 0);
  int one = 1, two = 2, three = 3, four = 4;
  PackedHashSetInsert(&set, &three, sizeof(int));
  PackedHashSetInsert(&set, &four, sizeof(int));
  PackedHashSetInsert(&set, &two, sizeof(int));
  PackedHashSetInsert(&set, &one, sizeof(int));
  ASSERT_INT_EQ(*(int*)PackedHashSetGet(&set, 0), 3);
  ASSERT_INT_EQ(*(int*)PackedHashSetGet(&set, 1), 4);
  ASSERT_INT_EQ(*(int*)PackedHashSetGet(&set, 2), 2);
  ASSERT_INT_EQ(*(int*)PackedHashSetGet(&set, 3), 1);  
  PackedHashSetSortByHash(&set);
  ASSERT_INT_EQ(*(int*)PackedHashSetGet(&set, 0), 1);
  ASSERT_INT_EQ(*(int*)PackedHashSetGet(&set, 1), 2);
  ASSERT_INT_EQ(*(int*)PackedHashSetGet(&set, 2), 3);
  ASSERT_INT_EQ(*(int*)PackedHashSetGet(&set, 3), 4);
  PackedHashSetDispose(&set);
}

// PackedHashSet doubles

int HashChar(const void* pt) {
  return *(char*)pt;
}

TEST(Chars_Empty) {
  PackedHashSet set;
  PackedHashSetInit(&set, HashChar, /*free_fn=*/NULL);
  ASSERT_CHAR_EQ(set.num_elems, 0);
  PackedHashSetDispose(&set);
}

TEST(Chars_Basic) {
  PackedHashSet set;
  PackedHashSetInit(&set, HashChar, /*free_fn=*/NULL);
  ASSERT_CHAR_EQ(set.num_elems, 0);
  char one = 1, two = 2, three = 3;
  ASSERT_TRUE(PackedHashSetInsert(&set, &one, sizeof(char)));
  ASSERT_CHAR_EQ(set.num_elems, 1);
  ASSERT_TRUE(PackedHashSetInsert(&set, &two, sizeof(char)));
  ASSERT_CHAR_EQ(set.num_elems, 2);
  ASSERT_TRUE(PackedHashSetInsert(&set, &three, sizeof(char)));
  ASSERT_CHAR_EQ(set.num_elems, 3);
  PackedHashSetDispose(&set);
}

TEST(Chars_Sort) {
  PackedHashSet set;
  PackedHashSetInit(&set, HashChar, /*free_fn=*/NULL);
  ASSERT_CHAR_EQ(set.num_elems, 0);
  char one = 1, two = 2, three = 3, four = 4;
  PackedHashSetInsert(&set, &three, sizeof(char));
  PackedHashSetInsert(&set, &four, sizeof(char));
  PackedHashSetInsert(&set, &two, sizeof(char));
  PackedHashSetInsert(&set, &one, sizeof(char));
  ASSERT_CHAR_EQ(*(char*)PackedHashSetGet(&set, 0), 3);
  ASSERT_CHAR_EQ(*(char*)PackedHashSetGet(&set, 1), 4);
  ASSERT_CHAR_EQ(*(char*)PackedHashSetGet(&set, 2), 2);
  ASSERT_CHAR_EQ(*(char*)PackedHashSetGet(&set, 3), 1);  
  PackedHashSetSortByHash(&set);
  ASSERT_CHAR_EQ(*(char*)PackedHashSetGet(&set, 0), 1);
  ASSERT_CHAR_EQ(*(char*)PackedHashSetGet(&set, 1), 2);
  ASSERT_CHAR_EQ(*(char*)PackedHashSetGet(&set, 2), 3);
  ASSERT_CHAR_EQ(*(char*)PackedHashSetGet(&set, 3), 4);
  PackedHashSetDispose(&set);
}

TEST(Tuple_Empty) {
  TupleHashSet set;
  TupleHashSetInit(&set);
  ASSERT_INT_EQ(set.num_elems, 0);
  TupleHashSetDispose(&set);
}

// TupleHashSet pairs

TEST(Tuple_Pairs_Basic) {
  TupleHashSet set;
  TupleHashSetInit(&set);
  CharPair a, b;
  CharPairInit(&a, 1, 2);
  CharPairInit(&b, 3, 4);
  ASSERT_TRUE(TupleHashSetInsertCharPair(&set, &a));
  ASSERT_TRUE(TupleHashSetInsertCharPair(&set, &b));
  ASSERT_INT_EQ(set.num_elems, 2);
  const CharPair* x = TupleHashSetFind(&set, &a);
  const CharPair* y = TupleHashSetFind(&set, &b);
  ASSERT_TRUE(x != NULL && x != &a);
  ASSERT_TRUE(y != NULL && y != &b);
  ASSERT_CHAR_EQ(x->first, 1);
  ASSERT_CHAR_EQ(x->second, 2);
  ASSERT_CHAR_EQ(y->first, 3);
  ASSERT_CHAR_EQ(y->second, 4);
  TupleHashSetDispose(&set);
}

TEST(Tuple_Pairs_Sort) {
  TupleHashSet set;
  TupleHashSetInit(&set);
  CharPair a, b;
  CharPairInit(&a, 1, 2);
  CharPairInit(&b, 3, 4);
  ASSERT_TRUE(TupleHashSetInsertCharPair(&set, &b));
  ASSERT_TRUE(TupleHashSetInsertCharPair(&set, &a));
  ASSERT_INT_EQ(set.num_elems, 2);
  const CharPair* x = TupleHashSetGet(&set, 0);
  const CharPair* y = TupleHashSetGet(&set, 1);
  ASSERT_TRUE(x != NULL);
  ASSERT_TRUE(y != NULL);
  ASSERT_CHAR_EQ(x->first, 3);
  ASSERT_CHAR_EQ(x->second, 4);
  ASSERT_CHAR_EQ(y->first, 1);
  ASSERT_CHAR_EQ(y->second, 2);
  TupleHashSetSort(&set);
  ASSERT_INT_EQ(set.num_elems, 2);
  x = TupleHashSetGet(&set, 0);
  y = TupleHashSetGet(&set, 1);
  ASSERT_TRUE(x != NULL);
  ASSERT_TRUE(y != NULL);
  ASSERT_CHAR_EQ(x->first, 1);
  ASSERT_CHAR_EQ(x->second, 2);
  ASSERT_CHAR_EQ(y->first, 3);
  ASSERT_CHAR_EQ(y->second, 4);
  TupleHashSetDispose(&set);
}

// TupleHashSet triples

TEST(Tuple_Triples_Basic) {
  TupleHashSet set;
  TupleHashSetInit(&set);
  CharTriple a, b;
  CharTripleInit(&a, 1, 2, 3);
  CharTripleInit(&b, 4, 5, 6);
  ASSERT_TRUE(TupleHashSetInsertCharTriple(&set, &a));
  ASSERT_TRUE(TupleHashSetInsertCharTriple(&set, &b));
  ASSERT_INT_EQ(set.num_elems, 2);
  const CharTriple* x = TupleHashSetFind(&set, &a);
  const CharTriple* y = TupleHashSetFind(&set, &b);
  ASSERT_TRUE(x != NULL && x != &a);
  ASSERT_TRUE(y != NULL && y != &b);
  ASSERT_CHAR_EQ(x->first, 1);
  ASSERT_CHAR_EQ(x->second, 2);
  ASSERT_CHAR_EQ(x->third, 3);
  ASSERT_CHAR_EQ(y->first, 4);
  ASSERT_CHAR_EQ(y->second, 5);
  ASSERT_CHAR_EQ(y->third, 6);  
  TupleHashSetDispose(&set);
}

TEST(Tuple_Triples_Sort) {
  TupleHashSet set;
  TupleHashSetInit(&set);
  CharTriple a, b;
  CharTripleInit(&a, 1, 2, 3);
  CharTripleInit(&b, 4, 5, 6);
  ASSERT_TRUE(TupleHashSetInsertCharTriple(&set, &b));
  ASSERT_TRUE(TupleHashSetInsertCharTriple(&set, &a));
  ASSERT_INT_EQ(set.num_elems, 2);
  const CharTriple* x = TupleHashSetGet(&set, 0);
  const CharTriple* y = TupleHashSetGet(&set, 1);
  ASSERT_TRUE(x != NULL);
  ASSERT_TRUE(y != NULL);
  ASSERT_CHAR_EQ(x->first, 4);
  ASSERT_CHAR_EQ(x->second, 5);
  ASSERT_CHAR_EQ(x->third, 6);  
  ASSERT_CHAR_EQ(y->first, 1);
  ASSERT_CHAR_EQ(y->second, 2);
  ASSERT_CHAR_EQ(y->third, 3);  
  TupleHashSetSort(&set);
  ASSERT_INT_EQ(set.num_elems, 2);
  x = TupleHashSetGet(&set, 0);
  y = TupleHashSetGet(&set, 1);
  ASSERT_TRUE(x != NULL);
  ASSERT_TRUE(y != NULL);
  ASSERT_CHAR_EQ(x->first, 1);
  ASSERT_CHAR_EQ(x->second, 2);
  ASSERT_CHAR_EQ(x->third, 3);
  ASSERT_CHAR_EQ(y->first, 4);
  ASSERT_CHAR_EQ(y->second, 5);
  ASSERT_CHAR_EQ(y->third, 6);  
  TupleHashSetDispose(&set);
}

// TupleHashSet mix

TEST(Tuple_Mix_Basic) {
  TupleHashSet set;
  TupleHashSetInit(&set);
  CharPair a, b;
  CharPairInit(&a, 1, 2);
  CharPairInit(&b, 3, 4);  
  CharTriple c, d;
  CharTripleInit(&c, 1, 2, 3);
  CharTripleInit(&d, 4, 5, 6);
  // Append to set
  ASSERT_TRUE(TupleHashSetInsertCharPair(&set, &a));
  ASSERT_TRUE(TupleHashSetInsertCharTriple(&set, &c));
  ASSERT_TRUE(TupleHashSetInsertCharPair(&set, &b));
  ASSERT_TRUE(TupleHashSetInsertCharTriple(&set, &d));
  // Asserts
  ASSERT_INT_EQ(set.num_elems, 4);
  const CharPair* x = TupleHashSetFind(&set, &a);
  const CharPair* y = TupleHashSetFind(&set, &b);
  const CharTriple* z = TupleHashSetFind(&set, &c);
  const CharTriple* w = TupleHashSetFind(&set, &d);  
  ASSERT_TRUE(x != NULL && x != &a);
  ASSERT_TRUE(y != NULL && y != &b);
  ASSERT_TRUE(z != NULL && z != &c);
  ASSERT_TRUE(w != NULL && w != &d);  
  // x
  ASSERT_CHAR_EQ(x->first, 1);
  ASSERT_CHAR_EQ(x->second, 2);
  // y
  ASSERT_CHAR_EQ(y->first, 3);
  ASSERT_CHAR_EQ(y->second, 4);
  // z
  ASSERT_CHAR_EQ(z->first, 1);
  ASSERT_CHAR_EQ(z->second, 2);
  ASSERT_CHAR_EQ(z->third, 3);
  // w
  ASSERT_CHAR_EQ(w->first, 4);
  ASSERT_CHAR_EQ(w->second, 5);
  ASSERT_CHAR_EQ(w->third, 6);  
  TupleHashSetDispose(&set);
}

TEST(Tuple_Mix_Sort) {
  TupleHashSet set;
  TupleHashSetInit(&set);
  CharPair a, b;
  CharPairInit(&a, 1, 2);
  CharPairInit(&b, 3, 4);  
  CharTriple c, d;
  CharTripleInit(&c, 1, 2, 3);
  CharTripleInit(&d, 4, 5, 6);
  // Append to set
  ASSERT_TRUE(TupleHashSetInsertCharPair(&set, &a));
  ASSERT_TRUE(TupleHashSetInsertCharTriple(&set, &c));
  ASSERT_TRUE(TupleHashSetInsertCharPair(&set, &b));
  ASSERT_TRUE(TupleHashSetInsertCharTriple(&set, &d));
  // Asserts
  ASSERT_INT_EQ(set.num_elems, 4);
  const CharPair* x = TupleHashSetGet(&set, 0);
  const CharPair* y = TupleHashSetGet(&set, 2);
  const CharTriple* z = TupleHashSetGet(&set, 1);
  const CharTriple* w = TupleHashSetGet(&set, 3);  
  ASSERT_TRUE(x != NULL && x != &a);
  ASSERT_TRUE(y != NULL && y != &b);
  ASSERT_TRUE(z != NULL && z != &c);
  ASSERT_TRUE(w != NULL && w != &d);  
  // x
  ASSERT_CHAR_EQ(x->first, 1);
  ASSERT_CHAR_EQ(x->second, 2);
  // y
  ASSERT_CHAR_EQ(y->first, 3);
  ASSERT_CHAR_EQ(y->second, 4);
  // z
  ASSERT_CHAR_EQ(z->first, 1);
  ASSERT_CHAR_EQ(z->second, 2);
  ASSERT_CHAR_EQ(z->third, 3);
  // w
  ASSERT_CHAR_EQ(w->first, 4);
  ASSERT_CHAR_EQ(w->second, 5);
  ASSERT_CHAR_EQ(w->third, 6);
  // Sort
  TupleHashSetSort(&set);
  // Asserts after sort
  ASSERT_INT_EQ(set.num_elems, 4);
  x = TupleHashSetGet(&set, 0);
  y = TupleHashSetGet(&set, 1);
  z = TupleHashSetGet(&set, 2);
  w = TupleHashSetGet(&set, 3);  
  ASSERT_TRUE(x != NULL && x != &a);
  ASSERT_TRUE(y != NULL && y != &b);
  ASSERT_TRUE(z != NULL && z != &c);
  ASSERT_TRUE(w != NULL && w != &d);
  // x
  ASSERT_CHAR_EQ(x->first, 1);
  ASSERT_CHAR_EQ(x->second, 2);
  // y
  ASSERT_CHAR_EQ(y->first, 3);
  ASSERT_CHAR_EQ(y->second, 4);
  // z
  ASSERT_CHAR_EQ(z->first, 1);
  ASSERT_CHAR_EQ(z->second, 2);
  ASSERT_CHAR_EQ(z->third, 3);
  // w
  ASSERT_CHAR_EQ(w->first, 4);
  ASSERT_CHAR_EQ(w->second, 5);
  ASSERT_CHAR_EQ(w->third, 6);
  TupleHashSetDispose(&set);
}

int main(int argc, char* argv[]) {
  LUnitOpts opts;
  LUnitOptsInit(&opts, argc, argv);
  RUN_TEST(Ints_Empty, &opts);
  RUN_TEST(Ints_Basic, &opts);
  RUN_TEST(Ints_Sort, &opts);
  RUN_TEST(Chars_Empty, &opts);
  RUN_TEST(Chars_Basic, &opts);
  RUN_TEST(Chars_Sort, &opts);
  RUN_TEST(Tuple_Empty, &opts);
  RUN_TEST(Tuple_Pairs_Basic, &opts);
  RUN_TEST(Tuple_Pairs_Sort, &opts);
  RUN_TEST(Tuple_Triples_Basic, &opts);
  RUN_TEST(Tuple_Triples_Sort, &opts);
  RUN_TEST(Tuple_Mix_Basic, &opts);
  RUN_TEST(Tuple_Mix_Sort, &opts);
  return 0;
}
