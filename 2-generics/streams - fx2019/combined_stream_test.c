#include "combined_stream.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASSERT(cmp) {					\
    if (!(cmp)) {						\
      printf("Assertion failed: %s\n", #cmp);	\
      return false;						\
    }								\
  }

#define TEST(t) { \
    printf("-- TESTING: %s\n", #t); \
    if (t()) {	  \
      printf("TEST %s: SUCCEEDED\n", #t);	\
    } \
    else { \
      printf("TEST %s: FAILED\n", #t);		\
    } \
    printf("####\n\n"); \
  }

// ---------------
// INTEGERS
// ---------------

int IntCmp(void* a, void* b) {
  return *(int*)a - *(int*)b;
}

bool SingleStream() {
  Stream s[1];
  // Stream: 0 1 2 3 4 ... 9
  StreamInitWithIntegers(&s[0], 0, 10, 1);
  CombinedStream cs;
  CombinedStreamInit(&cs, 1, s, IntCmp, free);
  int* val;
  for (int i = 0; i < 10; ++i) {
    val = CombinedStreamGetNext(&cs);
    ASSERT(val != NULL);
    ASSERT(*val == i);
    free(val);
  }
  ASSERT(CombinedStreamGetNext(&cs) == NULL);
  CombinedStreamDestroy(&cs);
  return true;
}

bool SingleStreamDoNotConsumeFully() {
  Stream s[1];
  // Stream: 0 1 2 3 4 ... 9
  StreamInitWithIntegers(&s[0], 0, 10, 1);
  CombinedStream cs;
  CombinedStreamInit(&cs, 1, s, IntCmp, free);
  int* val;
  for (int i = 0; i < 5; ++i) {
    val = CombinedStreamGetNext(&cs);
    ASSERT(val != NULL);
    ASSERT(*val == i);
    free(val);
  }
  CombinedStreamDestroy(&cs);
  return true;
}

bool TwoStreams() {
  Stream s[2];
  // Stream: 0 2 4 6 8
  StreamInitWithIntegers(&s[0], 0, 10, 2);
  // Stream: 1 3 5 7 9
  StreamInitWithIntegers(&s[1], 1, 10, 2);
  CombinedStream cs;
  CombinedStreamInit(&cs, 2, s, IntCmp, free);
  int* val;
  for (int i = 0; i < 10; ++i) {
    val = CombinedStreamGetNext(&cs);
    ASSERT(val != NULL);
    ASSERT(*val == i);
    free(val);
  }
  ASSERT(CombinedStreamGetNext(&cs) == NULL);
  CombinedStreamDestroy(&cs);
  return true;
}

bool MultipleStreams() {
  Stream s[10];
  // Stream: 0 10 20 30 ... 90
  // Stream: 1 11 21 31 ... 91
  // ..
  // Stream: 9 19 29 39 ... 99
  for (int i = 0; i < 10; ++i) {
    StreamInitWithIntegers(&s[i], i, 100, 10);
  }
  CombinedStream cs;
  CombinedStreamInit(&cs, 10, s, IntCmp, free);
  int* val;
  for (int i = 0; i < 100; ++i) {
    val = CombinedStreamGetNext(&cs);
    ASSERT(val != NULL);
    ASSERT(*val == i);
    free(val);
  }
  ASSERT(CombinedStreamGetNext(&cs) == NULL);
  CombinedStreamDestroy(&cs);
  return true;
}

bool NonOverlapingStreams() {
  Stream s[10];
  // Stream: 0 1 2 3 ... 9
  // Stream: 10 11 12 13 ... 19
  // ...
  // Stream: 90 91 92 ... 99
  for (int i = 0; i < 10; ++i) {
    StreamInitWithIntegers(&s[i], 10 * i, 10 * (i + 1), 1);
  }
  CombinedStream cs;
  CombinedStreamInit(&cs, 10, s, IntCmp, free);
  int* val;
  for (int i = 0; i < 100; ++i) {
    val = CombinedStreamGetNext(&cs);
    ASSERT(val != NULL);
    ASSERT(*val == i);
    free(val);
  }
  ASSERT(CombinedStreamGetNext(&cs) == NULL);
  CombinedStreamDestroy(&cs);
  return true;
}

bool NonOverlapingStreamsReversed() {
  Stream s[10];
  // Stream: 90 91 92 ... 99
  // Stream: 80 81 82 ... 99
  // ...
  // Stream: 0 1 2 3 ... 9
  for (int i = 0; i < 10; ++i) {
    StreamInitWithIntegers(&s[10 - i - 1], 10 * i, 10 * (i + 1), 1);
  }
  CombinedStream cs;
  CombinedStreamInit(&cs, 10, s, IntCmp, free);
  void* val;
  for (int i = 0; i < 100; ++i) {
    val = CombinedStreamGetNext(&cs);
    ASSERT(val != NULL);
    ASSERT(*(int*)val == i);
    free(val);
  }
  ASSERT(CombinedStreamGetNext(&cs) == NULL);
  CombinedStreamDestroy(&cs);
  return true;
}

bool EmptyStreams() {
  Stream s[1000];
  // Stream: empty, returns NULL on the first call
  for (int i = 0; i < 1000; ++i) {
    StreamInitWithIntegers(&s[i], 1, 0, 1);
  }
  CombinedStream cs;
  CombinedStreamInit(&cs, 1000, s, IntCmp, free);
  ASSERT(CombinedStreamGetNext(&cs) == NULL);
  CombinedStreamDestroy(&cs);
  return true;
}

// ---------------
// STRINGS
// ---------------

int StrCmp(void* a, void* b) {
  return strcmp(a, b);
}

void StrFree(void* a) {
  free(*(char**)a);
}

bool StringsSingleStream() {
  Stream s[1];
  StreamInitWithStrings(&s[0], 5, (char*[]){
      strdup("aba"),
	strdup("abc"),
	strdup("gi"),
	strdup("gio"),
	strdup("xyz")});
  CombinedStream cs;
  CombinedStreamInit(&cs, 1, s, StrCmp, free);
  char* val = CombinedStreamGetNext(&cs);
  ASSERT(strcmp((char*)val, "aba") == 0);
  free(val);
  val = CombinedStreamGetNext(&cs);
  ASSERT(strcmp((char*)val, "abc") == 0);
  free(val);
  val = CombinedStreamGetNext(&cs);
  ASSERT(strcmp((char*)val, "gi") == 0);
  free(val);
  val = CombinedStreamGetNext(&cs);
  ASSERT(strcmp((char*)val, "gio") == 0);
  free(val);
  CombinedStreamDestroy(&cs);
  return true;
}

bool StringsSingleStreamNoFree() {
  Stream s[1];
  StreamInitWithStringsNoFree(&s[0], 5, (char*[]){
      "aba",
	"abc",
	"gi",
	"gio",
	"xyz"});
  CombinedStream cs;
  CombinedStreamInit(&cs, 1, s, StrCmp, /*free_fn=*/NULL);
  char* val = CombinedStreamGetNext(&cs);
  ASSERT(strcmp((char*)val, "aba") == 0);
  val = CombinedStreamGetNext(&cs);
  ASSERT(strcmp((char*)val, "abc") == 0);
  val = CombinedStreamGetNext(&cs);
  ASSERT(strcmp((char*)val, "gi") == 0);
  val = CombinedStreamGetNext(&cs);
  ASSERT(strcmp((char*)val, "gio") == 0);
  CombinedStreamDestroy(&cs);
  return true;
}

bool StringsMultipleStreamNoFree() {
  Stream s[3];
  StreamInitWithStringsNoFree(&s[0], 3, (char*[]){
      "a", "c", "g"});
  StreamInitWithStringsNoFree(&s[1], 4, (char*[]){
      "ab", "bc", "h", "xyz"});
  StreamInitWithStringsNoFree(&s[2], 2, (char*[]){
      "ba", "cc"});
  CombinedStream cs;
  CombinedStreamInit(&cs, 3, s, StrCmp, /*free_fn=*/NULL);
  char* val = CombinedStreamGetNext(&cs);
  ASSERT(strcmp((char*)val, "a") == 0);
  val = CombinedStreamGetNext(&cs);
  ASSERT(strcmp((char*)val, "ab") == 0);
  val = CombinedStreamGetNext(&cs);
  ASSERT(strcmp((char*)val, "ba") == 0);
  val = CombinedStreamGetNext(&cs);
  ASSERT(strcmp((char*)val, "bc") == 0);
  val = CombinedStreamGetNext(&cs);
  ASSERT(strcmp((char*)val, "c") == 0);
  val = CombinedStreamGetNext(&cs);
  ASSERT(strcmp((char*)val, "cc") == 0);
  val = CombinedStreamGetNext(&cs);
  ASSERT(strcmp((char*)val, "g") == 0);
  val = CombinedStreamGetNext(&cs);
  ASSERT(strcmp((char*)val, "h") == 0);
  val = CombinedStreamGetNext(&cs);
  ASSERT(strcmp((char*)val, "xyz") == 0);
  ASSERT(CombinedStreamGetNext(&cs) == NULL);
  CombinedStreamDestroy(&cs);
  return true;
}

// ---------------
// STRUCTS
// ---------------

int PairCmp(void* a, void* b) {
  Pair* p = a;
  Pair* q = b;
  if (p->x != q->x) {
    return p->x - q->x;
  }
  return p->y - q->y;
}

bool StructsSingleStream() {
  Stream s[1];
  StreamInitWithPairs(&s[0], 3, (Pair[]){
      P(1, 2), P(2, 3), P(4, 5)});
  CombinedStream cs;
  CombinedStreamInit(&cs, 1, s, PairCmp, /*free_fn=*/NULL);
  Pair* val = CombinedStreamGetNext(&cs);
  ASSERT(val->x == 1 && val->y == 2);
  val = CombinedStreamGetNext(&cs);
  ASSERT(val->x == 2 && val->y == 3);
  val = CombinedStreamGetNext(&cs);
  ASSERT(val->x == 4 && val->y == 5);
  ASSERT(CombinedStreamGetNext(&cs) == NULL);
  CombinedStreamDestroy(&cs);
  return true;
}

bool StructsMultipleStreams() {
  Stream s[2];
  StreamInitWithPairs(&s[0], 3, (Pair[]){
      P(1, 2), P(2, 3), P(4, 5)});
  StreamInitWithPairs(&s[1], 4, (Pair[]){
      P(1, 0), P(2, 4), P(3, 5), P(4, 1)});
  CombinedStream cs;
  CombinedStreamInit(&cs, 2, s, PairCmp, /*free_fn=*/NULL);
  Pair* val = CombinedStreamGetNext(&cs);
  ASSERT(val->x == 1 && val->y == 0);  
  val = CombinedStreamGetNext(&cs);
  ASSERT(val->x == 1 && val->y == 2);
  val = CombinedStreamGetNext(&cs);
  ASSERT(val->x == 2 && val->y == 3);
  val = CombinedStreamGetNext(&cs);
  ASSERT(val->x == 2 && val->y == 4);
  val = CombinedStreamGetNext(&cs);
  ASSERT(val->x == 3 && val->y == 5);
  val = CombinedStreamGetNext(&cs);
  ASSERT(val->x == 4 && val->y == 1);
  val = CombinedStreamGetNext(&cs);  
  ASSERT(val->x == 4 && val->y == 5);
  ASSERT(CombinedStreamGetNext(&cs) == NULL);
  CombinedStreamDestroy(&cs);
  return true;
}

// ---------------
// LARGE INPUT
// ---------------

bool FewStreamsWithManyElementsEach() {
  Stream s[10];
  for (int i = 0; i < 10; ++i) {
    StreamInitWithIntegers(&s[i], 0, 1000000, 1);
  }
  CombinedStream cs;
  CombinedStreamInit(&cs, 10, s, IntCmp, free);
  for (int i = 0; i < 1000000; ++i) {
    for (int j = 0; j < 10; ++j) {
      int* val = CombinedStreamGetNext(&cs);
      ASSERT(*val == i);
    }
  }
  ASSERT(CombinedStreamGetNext(&cs) == NULL);
  CombinedStreamDestroy(&cs);
  return true;
}

bool ManyStreamsWithFewElementsEach() {
  Stream* s = malloc(sizeof(Stream) * 1000000);
  for (int i = 0; i < 1000000; ++i) {
    StreamInitWithIntegers(&s[i], 0, 10, 1);
  }
  CombinedStream cs;
  CombinedStreamInit(&cs, 1000000, s, IntCmp, free);
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 1000000; ++j) {
      int* val = CombinedStreamGetNext(&cs);
      ASSERT(*val == i);
    }
  }
  ASSERT(CombinedStreamGetNext(&cs) == NULL);
  CombinedStreamDestroy(&cs);
  free(s);
  return true;
}

bool AverageNumberOfStreamsWithAverageNumberOfItems() {
  Stream* s = malloc(sizeof(Stream) * 1000);
  for (int i = 0; i < 1000; ++i) {
    StreamInitWithIntegers(&s[i], 0, 10000, 1);
  }
  CombinedStream cs;
  CombinedStreamInit(&cs, 1000, s, IntCmp, free);
  for (int i = 0; i < 10000; ++i) {
    for (int j = 0; j < 1000; ++j) {
      int* val = CombinedStreamGetNext(&cs);
      ASSERT(*val == i);
    }
  }
  ASSERT(CombinedStreamGetNext(&cs) == NULL);
  CombinedStreamDestroy(&cs);
  free(s);
  return true;
}

// ---------------
// HUGE INPUT
// ---------------

bool HugeInput() {
  Stream* s = malloc(sizeof(Stream) * 1000);
  for (int i = 0; i < 1000; ++i) {
    StreamInitWithIntegers(&s[i], 0, 1000000000, 1);
  }
  CombinedStream cs;
  CombinedStreamInit(&cs, 1000, s, IntCmp, free);
  int cnt = 0;
  // Checks only first 1000000 items.
  for (int i = 0; i < 1000000000; ++i) {
    for (int j = 0; j < 1000; ++j) {
      int* val = CombinedStreamGetNext(&cs);
      ASSERT(*val == i);
      ++cnt;
      if (cnt == 1000000) {
	break;
      }      
    }
    if (cnt == 1000000) {
      break;
    }
  }
  CombinedStreamDestroy(&cs);
  free(s);
  return true;
}


int main(int argc, char** argv) {
  // INTEGERS
  TEST(SingleStream);
  TEST(SingleStreamDoNotConsumeFully);
  TEST(TwoStreams);
  TEST(MultipleStreams);
  TEST(NonOverlapingStreams);
  TEST(NonOverlapingStreamsReversed);
  TEST(EmptyStreams);
  // STRINGS
  TEST(StringsSingleStream);
  TEST(StringsSingleStreamNoFree);
  TEST(StringsMultipleStreamNoFree);
  // STRUCTS
  TEST(StructsSingleStream);
  TEST(StructsMultipleStreams);

  // Uncommet tests bellow if you want to check your code on big data.

  // LARGE INPUT
  /* TEST(FewStreamsWithManyElementsEach); */
  /* TEST(ManyStreamsWithFewElementsEach); */
  /* TEST(AverageNumberOfStreamsWithAverageNumberOfItems); */
  // HUGE INPUT
  /* TEST(HugeInput); */
  return 0;
}
