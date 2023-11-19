#include "sorted_multi_set.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define RUN(x) RunTest(#x, x);

#define ASSERT(expr) {							\
  if (!(expr)) {								\
    printf("%s:%d: Assertion failed: %s\n", __FILE__, __LINE__, #expr);	\
    return false;							\
  }									\
  }

void RunTest(char* name, bool(*t)()) {
  printf("++ Running: %s\n", name);
  bool res = t();
  printf("-- Test %s: %s\n", name, res ? "SUCCEEDED" : "FAILED");
  printf("\n\n");
}

bool Empty() {
  SortedMultiSet s;
  SortedMultiSetInit(&s, 1, NULL, NULL);
  ASSERT(0 == SortedMultiSetSize(&s));
  SortedMultiSetDestroy(&s);
  return true;
}

int IntCmp(void* a, void* b) {
  return *(int*)a - *(int*)b;
}

bool IntsInsert() {
  SortedMultiSet s;
  SortedMultiSetInit(&s, sizeof(int), IntCmp, NULL);
  int a = 1, b = 2, c = 3;
  SortedMultiSetInsert(&s, &a);
  ASSERT(1 == SortedMultiSetSize(&s));
  ASSERT(1 == *(int*)SortedMultiSetGet(&s, 0));
  ASSERT(1 == SortedMultiSetGetCount(&s, 0));
  SortedMultiSetInsert(&s, &b);
  ASSERT(2 == SortedMultiSetSize(&s));
  ASSERT(1 == *(int*)SortedMultiSetGet(&s, 0));
  ASSERT(2 == *(int*)SortedMultiSetGet(&s, 1));
  ASSERT(1 == SortedMultiSetGetCount(&s, 0));  
  ASSERT(1 == SortedMultiSetGetCount(&s, 1));  
  SortedMultiSetInsert(&s, &c);
  ASSERT(3 == SortedMultiSetSize(&s));
  ASSERT(1 == *(int*)SortedMultiSetGet(&s, 0));
  ASSERT(2 == *(int*)SortedMultiSetGet(&s, 1));
  ASSERT(3 == *(int*)SortedMultiSetGet(&s, 2));
  ASSERT(1 == SortedMultiSetGetCount(&s, 0));  
  ASSERT(1 == SortedMultiSetGetCount(&s, 1));
  ASSERT(1 == SortedMultiSetGetCount(&s, 2));
  SortedMultiSetDestroy(&s);
  return true;
}

bool IntsInsertUnordered() {
  SortedMultiSet s;
  SortedMultiSetInit(&s, sizeof(int), IntCmp, NULL);
  int a = 1, b = 2, c = 3;
  SortedMultiSetInsert(&s, &b);
  ASSERT(1 == SortedMultiSetSize(&s));
  ASSERT(2 == *(int*)SortedMultiSetGet(&s, 0));
  ASSERT(1 == SortedMultiSetGetCount(&s, 0));  
  SortedMultiSetInsert(&s, &a);
  ASSERT(2 == SortedMultiSetSize(&s));
  ASSERT(1 == *(int*)SortedMultiSetGet(&s, 0));
  ASSERT(2 == *(int*)SortedMultiSetGet(&s, 1));
  ASSERT(1 == SortedMultiSetGetCount(&s, 0));  
  ASSERT(1 == SortedMultiSetGetCount(&s, 1));
  SortedMultiSetInsert(&s, &c);
  ASSERT(3 == SortedMultiSetSize(&s));
  ASSERT(1 == *(int*)SortedMultiSetGet(&s, 0));
  ASSERT(2 == *(int*)SortedMultiSetGet(&s, 1));
  ASSERT(3 == *(int*)SortedMultiSetGet(&s, 2));
  ASSERT(1 == SortedMultiSetGetCount(&s, 0));  
  ASSERT(1 == SortedMultiSetGetCount(&s, 1));
  ASSERT(1 == SortedMultiSetGetCount(&s, 2));
  SortedMultiSetDestroy(&s);
  return true;
}

bool IntsDuplicatesRemove() {
  SortedMultiSet s;
  SortedMultiSetInit(&s, sizeof(int), IntCmp, NULL);
  int a = 1, b = 2;
  SortedMultiSetInsert(&s, &b);
  ASSERT(1 == SortedMultiSetSize(&s));
  SortedMultiSetInsert(&s, &a);
  ASSERT(2 == SortedMultiSetSize(&s));
  SortedMultiSetInsert(&s, &a);
  ASSERT(2 == SortedMultiSetSize(&s));
  ASSERT(1 == *(int*)SortedMultiSetGet(&s, 0));
  ASSERT(2 == *(int*)SortedMultiSetGet(&s, 1));
  ASSERT(2 == SortedMultiSetGetCount(&s, 0));  
  ASSERT(1 == SortedMultiSetGetCount(&s, 1));
  int x = 1, y = 2;
  SortedMultiSetRemove(&s, &x);
  ASSERT(2 == SortedMultiSetSize(&s));
  ASSERT(1 == *(int*)SortedMultiSetGet(&s, 0));
  ASSERT(2 == *(int*)SortedMultiSetGet(&s, 1));
  ASSERT(1 == SortedMultiSetGetCount(&s, 0));  
  ASSERT(1 == SortedMultiSetGetCount(&s, 1));
  SortedMultiSetRemove(&s, &x);
  ASSERT(1 == SortedMultiSetSize(&s));
  ASSERT(2 == *(int*)SortedMultiSetGet(&s, 0));
  ASSERT(1 == SortedMultiSetGetCount(&s, 0));
  SortedMultiSetRemove(&s, &y);
  ASSERT(0 == SortedMultiSetSize(&s));
  return true;
}

int StrCmp(void* a, void* b) {
  return strcmp(*(char**)a, *(char**)b);
}

void StrFree(void* ptr) {
  free(*(char**)ptr);
}

bool Strings() {
  SortedMultiSet s;
  SortedMultiSetInit(&s, sizeof(char*), StrCmp, StrFree);
  char *a = strdup("abc"), *b = strdup("xyz");
  SortedMultiSetInsert(&s, &b);
  SortedMultiSetInsert(&s, &a);
  ASSERT(2 == SortedMultiSetSize(&s));
  ASSERT(0 == strcmp("abc", *(char**)SortedMultiSetGet(&s, 0)));
  ASSERT(0 == strcmp("xyz", *(char**)SortedMultiSetGet(&s, 1)));
  ASSERT(1 == SortedMultiSetGetCount(&s, 0));  
  ASSERT(1 == SortedMultiSetGetCount(&s, 1));
  SortedMultiSetDestroy(&s);
  return true;
}

bool StringsDuplicate() {
  SortedMultiSet s;
  SortedMultiSetInit(&s, sizeof(char*), StrCmp, StrFree);
  char *a = strdup("abc"), *b = strdup("xyz");
  char *c = strdup("abc");
  SortedMultiSetInsert(&s, &b);
  SortedMultiSetInsert(&s, &a);
  SortedMultiSetInsert(&s, &c);
  ASSERT(2 == SortedMultiSetSize(&s));
  ASSERT(0 == strcmp("abc", *(char**)SortedMultiSetGet(&s, 0)));
  ASSERT(0 == strcmp("xyz", *(char**)SortedMultiSetGet(&s, 1)));
  ASSERT(2 == SortedMultiSetGetCount(&s, 0));  
  ASSERT(1 == SortedMultiSetGetCount(&s, 1));
  char *x = "abc", *y = "xyz";
  SortedMultiSetRemove(&s, &x);
  ASSERT(2 == SortedMultiSetSize(&s));
  ASSERT(0 == strcmp("abc", *(char**)SortedMultiSetGet(&s, 0)));
  ASSERT(0 == strcmp("xyz", *(char**)SortedMultiSetGet(&s, 1)));
  ASSERT(1 == SortedMultiSetGetCount(&s, 0));  
  ASSERT(1 == SortedMultiSetGetCount(&s, 1));
  SortedMultiSetRemove(&s, &y);
  ASSERT(1 == SortedMultiSetSize(&s));
  ASSERT(0 == strcmp("abc", *(char**)SortedMultiSetGet(&s, 0)));
  ASSERT(1 == SortedMultiSetGetCount(&s, 0));  
  SortedMultiSetRemove(&s, &x);
  ASSERT(0 == SortedMultiSetSize(&s));
  SortedMultiSetDestroy(&s);
  return true;
}

int main() {
  RUN(Empty);
  RUN(IntsInsert);
  RUN(IntsInsertUnordered);
  RUN(IntsDuplicatesRemove);
  RUN(Strings);
  RUN(StringsDuplicate);
  return 0;
}
