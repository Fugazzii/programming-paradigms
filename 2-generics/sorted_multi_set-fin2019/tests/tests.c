#include "../sorted_multi_set.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "gl/lunit/lunit.h"

typedef struct {
  int a;
  int b;
} P;

P NewP(int a, int b) {
  P p;
  p.a = a;
  p.b = b;
  return p;
}

int PCmp(void* a, void* b) {
  P* x = a;
  P* y = b;
  if (x->a != y->a) {
    return x->a - y->a;
  }
  return x->b - y->b;
}

bool PairEq(char* msg, P* actual, P expected) {
  if (PCmp(actual, &expected) == 0) {
    return true;
  }
  sprintf(msg, "Expected (%d, %d) got (%d, %d) instead.", expected.a,
	  expected.b, actual->a, actual->b);
  return false;
}

#define ASSERT_PAIR_EQ(actual, expected) \
  __ASSERT_EQ(PairEq, PairEqMsg, actual, expected)

TEST(Empty) {
  SortedMultiSet s;
  SortedMultiSetInit(&s, 1, NULL, NULL);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 0);
  SortedMultiSetDestroy(&s);
}

int IntCmp(void* a, void* b) {
  return *(int*)a - *(int*)b;
}

TEST(IntsInsert) {
  SortedMultiSet s;
  SortedMultiSetInit(&s, sizeof(int), IntCmp, NULL);
  int a = 1, b = 2, c = 3;
  SortedMultiSetInsert(&s, &a);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 1);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 0), 1);
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 1);
  SortedMultiSetInsert(&s, &b);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 2);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 0), 1);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 1), 2);
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 1);
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 1);  
  SortedMultiSetInsert(&s, &c);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 3);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 0), 1);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 1), 2);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 2), 3);
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 1);  
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 1);
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 2), 1);
  SortedMultiSetDestroy(&s);
}

TEST(IntsInsertDuplicates) {
  SortedMultiSet s;
  SortedMultiSetInit(&s, sizeof(int), IntCmp, NULL);
  int a = 1, b = 2, c = 3;
  int x = 1, y = 2, z = 3;
  // Insert 1
  SortedMultiSetInsert(&s, &a);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 1);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 0), 1);
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 1);
  // Insert 1
  SortedMultiSetInsert(&s, &x);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 1);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 0), 1);
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 2);
  // Insert 2
  SortedMultiSetInsert(&s, &b);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 2);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 2);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 0), 1);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 1), 2);
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 2);  
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 1);
  // Insert 3
  SortedMultiSetInsert(&s, &c);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 3);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 0), 1);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 1), 2);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 2), 3);
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 2);  
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 1);
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 2), 1);
  // Insert 3 and 2
  SortedMultiSetInsert(&s, &z);
  SortedMultiSetInsert(&s, &y);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 3);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 0), 1);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 1), 2);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 2), 3);
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 2);  
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 2);
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 2), 2);  
  SortedMultiSetDestroy(&s);
}

TEST(IntsInsertUnordered) {
  SortedMultiSet s;
  SortedMultiSetInit(&s, sizeof(int), IntCmp, NULL);
  int a = 1, b = 2, c = 3;
  SortedMultiSetInsert(&s, &b);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 1);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 0), 2);
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 1);  
  SortedMultiSetInsert(&s, &a);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 2);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 0), 1);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 1), 2);
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 1);  
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 1);
  SortedMultiSetInsert(&s, &c);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 3);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 0), 1);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 1), 2);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 2), 3);
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 1);  
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 1);
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 2), 1);
  SortedMultiSetDestroy(&s);
}

TEST(IntsDuplicatesRemove) {
  SortedMultiSet s;
  SortedMultiSetInit(&s, sizeof(int), IntCmp, NULL);
  int a = 1, b = 2;
  SortedMultiSetInsert(&s, &b);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 1);
  SortedMultiSetInsert(&s, &a);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 2);
  SortedMultiSetInsert(&s, &a);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 2);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 0), 1);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 1), 2);
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 2);  
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 1);
  int x = 1;
  SortedMultiSetRemove(&s, &x);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 2);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 0), 1);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 1), 2);
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 1);  
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 1);
  SortedMultiSetDestroy(&s);
}

TEST(IntsDuplicatesRemoveAll) {
  SortedMultiSet s;
  SortedMultiSetInit(&s, sizeof(int), IntCmp, NULL);
  int a = 1, b = 2;
  SortedMultiSetInsert(&s, &b);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 1);
  SortedMultiSetInsert(&s, &a);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 2);
  SortedMultiSetInsert(&s, &a);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 2);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 0), 1);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 1), 2);
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 2);  
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 1);
  int x = 1, y = 2;
  SortedMultiSetRemove(&s, &x);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 2);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 0), 1);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 1), 2);
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 1);  
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 1);
  SortedMultiSetRemove(&s, &x);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 1);
  ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, 0), 2);
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 1);
  SortedMultiSetRemove(&s, &y);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 0);
  SortedMultiSetDestroy(&s);
}

int StrCmp(void* a, void* b) {
  return strcmp(*(char**)a, *(char**)b);
}

void StrFree(void* ptr) {
  free(*(char**)ptr);
}

TEST(StringsInsert) {
  SortedMultiSet s;
  SortedMultiSetInit(&s, sizeof(char*), StrCmp, StrFree);
  char *a = strdup("abc"), *b = strdup("xyz");
  SortedMultiSetInsert(&s, &a);
  SortedMultiSetInsert(&s, &b);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 2);
  ASSERT_STR_EQ(*(char**)SortedMultiSetGet(&s, 0), "abc");
  ASSERT_STR_EQ(*(char**)SortedMultiSetGet(&s, 1), "xyz");
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 1);  
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 1);
  SortedMultiSetDestroy(&s);
}

TEST(StringsInsertUnordered) {
  SortedMultiSet s;
  SortedMultiSetInit(&s, sizeof(char*), StrCmp, StrFree);
  char *a = strdup("abc"), *b = strdup("xyz");
  SortedMultiSetInsert(&s, &b);
  SortedMultiSetInsert(&s, &a);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 2);
  ASSERT_STR_EQ(*(char**)SortedMultiSetGet(&s, 0), "abc");
  ASSERT_STR_EQ(*(char**)SortedMultiSetGet(&s, 1), "xyz");
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 1);  
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 1);
  SortedMultiSetDestroy(&s);
}

TEST(StringsInsertDuplicates) {
  SortedMultiSet s;
  SortedMultiSetInit(&s, sizeof(char*), StrCmp, StrFree);
  char *a = strdup("abc"), *b = strdup("xyz");
  char *c = strdup("abc"), *d = strdup("xyz");  
  SortedMultiSetInsert(&s, &b);
  SortedMultiSetInsert(&s, &a);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 2);
  ASSERT_STR_EQ(*(char**)SortedMultiSetGet(&s, 0), "abc");
  ASSERT_STR_EQ(*(char**)SortedMultiSetGet(&s, 1), "xyz");
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 1);  
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 1);
  SortedMultiSetInsert(&s, &c);
  SortedMultiSetInsert(&s, &d);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 2);
  ASSERT_STR_EQ(*(char**)SortedMultiSetGet(&s, 0), "abc");
  ASSERT_STR_EQ(*(char**)SortedMultiSetGet(&s, 1), "xyz");
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 2);  
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 2);
  SortedMultiSetDestroy(&s);
}

TEST(StringsDuplicatesRemove) {
  SortedMultiSet s;
  SortedMultiSetInit(&s, sizeof(char*), StrCmp, StrFree);
  char *a = strdup("abc"), *b = strdup("xyz");
  char *c = strdup("abc");
  SortedMultiSetInsert(&s, &b);
  SortedMultiSetInsert(&s, &a);
  SortedMultiSetInsert(&s, &c);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 2);
  ASSERT_STR_EQ(*(char**)SortedMultiSetGet(&s, 0), "abc");
  ASSERT_STR_EQ(*(char**)SortedMultiSetGet(&s, 1), "xyz");
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 2);  
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 1);
  char *x = "abc";
  SortedMultiSetRemove(&s, &x);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 2);
  ASSERT_STR_EQ(*(char**)SortedMultiSetGet(&s, 0), "abc");
  ASSERT_STR_EQ(*(char**)SortedMultiSetGet(&s, 1), "xyz");
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 1);  
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 1);
  SortedMultiSetDestroy(&s);
}

TEST(StringsDuplicatesRemoveAll) {
  SortedMultiSet s;
  SortedMultiSetInit(&s, sizeof(char*), StrCmp, StrFree);
  char *a = strdup("abc"), *b = strdup("xyz");
  char *c = strdup("abc");
  SortedMultiSetInsert(&s, &b);
  SortedMultiSetInsert(&s, &a);
  SortedMultiSetInsert(&s, &c);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 2);
  ASSERT_STR_EQ(*(char**)SortedMultiSetGet(&s, 0), "abc");
  ASSERT_STR_EQ(*(char**)SortedMultiSetGet(&s, 1), "xyz");
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 2);  
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 1);
  char *x = "abc", *y = "xyz";
  SortedMultiSetRemove(&s, &x);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 2);
  ASSERT_STR_EQ(*(char**)SortedMultiSetGet(&s, 0), "abc");
  ASSERT_STR_EQ(*(char**)SortedMultiSetGet(&s, 1), "xyz");
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 1);  
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 1);
  SortedMultiSetRemove(&s, &y);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 1);
  ASSERT_STR_EQ(*(char**)SortedMultiSetGet(&s, 0), "abc");
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 1);  
  SortedMultiSetRemove(&s, &x);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 0);
  SortedMultiSetDestroy(&s);
}

TEST(PairsInsert) {
  SortedMultiSet s;
  SortedMultiSetInit(&s, sizeof(P), PCmp, NULL);
  P a = NewP(1, 2), b = NewP(1, 3), c = NewP(2, 1);
  SortedMultiSetInsert(&s, &a);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 1);
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 0), NewP(1, 2));
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 1);
  SortedMultiSetInsert(&s, &b);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 2);
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 0), NewP(1, 2));
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 1), NewP(1, 3));
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 1);
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 1);  
  SortedMultiSetInsert(&s, &c);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 3);
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 0), NewP(1, 2));
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 1), NewP(1, 3));
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 2), NewP(2, 1));
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 1);  
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 1);
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 2), 1);
  SortedMultiSetDestroy(&s);
}

TEST(PairsInsertDuplicates) {
  SortedMultiSet s;
  SortedMultiSetInit(&s, sizeof(P), PCmp, NULL);
  P a = NewP(1, 2), b = NewP(1, 3), c = NewP(2, 1);
  P x = NewP(1, 2), y = NewP(1, 3), z = NewP(2, 1);
  // Insert (1, 2)
  SortedMultiSetInsert(&s, &a);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 1);
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 0), NewP(1, 2));
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 1);
  // Insert (1, 2)
  SortedMultiSetInsert(&s, &x);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 1);
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 0), NewP(1, 2));
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 2);
  // Insert (1, 3)
  SortedMultiSetInsert(&s, &b);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 2);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 2);
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 0), NewP(1, 2));
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 1), NewP(1, 3));
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 2);  
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 1);
  // Insert (2, 1)
  SortedMultiSetInsert(&s, &c);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 3);
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 0), NewP(1, 2));
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 1), NewP(1, 3));
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 2), NewP(2, 1));
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 2);  
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 1);
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 2), 1);
  // Insert (2, 1) and (1, 3)
  SortedMultiSetInsert(&s, &z);
  SortedMultiSetInsert(&s, &y);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 3);
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 0), NewP(1, 2));
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 1), NewP(1, 3));
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 2), NewP(2, 1));
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 2);  
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 2);
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 2), 2);  
  SortedMultiSetDestroy(&s);
}

TEST(PairsInsertUnordered) {
  SortedMultiSet s;
  SortedMultiSetInit(&s, sizeof(P), PCmp, NULL);
  P a = NewP(1, 2), b = NewP(1, 3), c = NewP(2, 1);
  SortedMultiSetInsert(&s, &b);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 1);
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 0), NewP(1, 3));
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 1);  
  SortedMultiSetInsert(&s, &a);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 2);
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 0), NewP(1, 2));
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 1), NewP(1, 3));
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 1);  
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 1);
  SortedMultiSetInsert(&s, &c);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 3);
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 0), NewP(1, 2));
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 1), NewP(1, 3));
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 2), NewP(2, 1));
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 1);  
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 1);
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 2), 1);
  SortedMultiSetDestroy(&s);
}

TEST(PairsDuplicatesRemove) {
  SortedMultiSet s;
  SortedMultiSetInit(&s, sizeof(P), PCmp, NULL);
  P a = NewP(1, 2), b = NewP(3, 4);
  SortedMultiSetInsert(&s, &b);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 1);
  SortedMultiSetInsert(&s, &a);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 2);
  SortedMultiSetInsert(&s, &a);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 2);
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 0), NewP(1, 2));
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 1), NewP(3, 4));
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 2);  
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 1);
  P x = NewP(1, 2);
  SortedMultiSetRemove(&s, &x);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 2);
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 0), NewP(1, 2));
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 1), NewP(3, 4));
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 1);  
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 1);
  SortedMultiSetDestroy(&s);
}

TEST(PairsDuplicatesRemoveAll) {
  SortedMultiSet s;
  SortedMultiSetInit(&s, sizeof(P), PCmp, NULL);
  P a = NewP(1, 2), b = NewP(3, 4);
  SortedMultiSetInsert(&s, &b);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 1);
  SortedMultiSetInsert(&s, &a);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 2);
  SortedMultiSetInsert(&s, &a);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 2);
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 0), NewP(1, 2));
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 1), NewP(3, 4));
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 2);  
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 1);
  P x = NewP(1, 2), y = NewP(3 ,4);
  SortedMultiSetRemove(&s, &x);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 2);
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 0), NewP(1, 2));
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 1), NewP(3, 4));
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 1);  
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 1), 1);
  SortedMultiSetRemove(&s, &x);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 1);
  ASSERT_PAIR_EQ(SortedMultiSetGet(&s, 0), NewP(3, 4));
  ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 1);
  SortedMultiSetRemove(&s, &y);
  ASSERT_INT_EQ(SortedMultiSetSize(&s), 0);
  SortedMultiSetDestroy(&s);
}

TEST(IntsLarge) {
  SortedMultiSet s;
  SortedMultiSetInit(&s, sizeof(int), IntCmp, NULL);
  for (int i = 0; i < 100100; ++i) {
    SortedMultiSetInsert(&s, &i);
    ASSERT_INT_EQ(SortedMultiSetSize(&s), i + 1);
    ASSERT_INT_EQ(*(int*)SortedMultiSetGet(&s, i), i);
    ASSERT_INT_EQ(SortedMultiSetGetCount(&s, 0), 1);
  }
  SortedMultiSetDestroy(&s);
}

int main(int argc, char* argv[]) {
  LUnitOpts opts;
  LUnitOptsInit(&opts, argc, argv);

  TEST_SUITE_WITH_WEIGHT(insert_no_duplicates, 0.2);
  TEST_SUITE_WITH_WEIGHT(insert_duplicates, 0.2);
  TEST_SUITE_WITH_WEIGHT(remove_partially, 0.25);
  TEST_SUITE_WITH_WEIGHT(remove_fully, 0.30);
  TEST_SUITE_WITH_WEIGHT(large, 0.05);
  
  ADD_TEST(insert_no_duplicates, Empty);

  // Integers
  ADD_TEST(insert_no_duplicates, IntsInsert);
  ADD_TEST(insert_no_duplicates, IntsInsertUnordered);
  ADD_TEST(insert_duplicates, IntsInsertDuplicates);
  ADD_TEST(remove_partially, IntsDuplicatesRemove);
  ADD_TEST(remove_fully, IntsDuplicatesRemoveAll);

  // Strings
  ADD_TEST(insert_no_duplicates, StringsInsert);
  ADD_TEST(insert_no_duplicates, StringsInsertUnordered);  
  ADD_TEST(insert_no_duplicates, StringsInsertDuplicates);  
  ADD_TEST(remove_partially, StringsDuplicatesRemove);
  ADD_TEST(remove_fully, StringsDuplicatesRemoveAll);

  // Pairs
  ADD_TEST(insert_no_duplicates, PairsInsert);
  ADD_TEST(insert_no_duplicates, PairsInsertUnordered);  
  ADD_TEST(insert_no_duplicates, PairsInsertDuplicates);  
  ADD_TEST(remove_partially, PairsDuplicatesRemove);
  ADD_TEST(remove_fully, PairsDuplicatesRemoveAll);

  // Large
  ADD_TEST(large, IntsLarge);

  TestSuite* suites[] = {&insert_no_duplicates, &insert_duplicates,
			 &remove_partially, &remove_fully, &large};
  ProcessTestSuites(5, suites, &opts);
  TestSuiteDispose(&insert_no_duplicates);
  TestSuiteDispose(&insert_duplicates);
  TestSuiteDispose(&remove_partially);
  TestSuiteDispose(&remove_fully); 
  TestSuiteDispose(&large); 
  return 0;
}
