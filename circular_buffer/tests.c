#include "circular_buffer.h"
#include "student_buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ASSERT_INT_EQ(a, b) {						\
    int expected = (a);							\
    int actual = (b);							\
    if (expected != actual) {						\
      printf("%s:%d Assertion failed: expected %d got %d\n", __FILE__, __LINE__, expected, actual); \
      return false;							\
    }									\
  }

#define ASSERT_STR_EQ(a, b) {						\
    char* expected = (a);							\
    char* actual = (b);							\
    if (strcmp(expected, actual) != 0) {					\
      printf("%s:%d Assertion failed: expected %s got %s\n", __FILE__, __LINE__, expected, actual); \
      return false;							\
    }									\
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

// CircularBuffer with integers

bool Basic() {
  CircularBuffer cb;
  CircularBufferInit(&cb, sizeof(int), 2, NULL, NULL);
  int x = 1;
  CircularBufferPush(&cb, &x);
  x = 2;
  CircularBufferPush(&cb, &x);
  CircularBufferPop(&cb, &x);
  ASSERT_INT_EQ(1, x);
  CircularBufferPop(&cb, &x);
  ASSERT_INT_EQ(2, x);
  CircularBufferDestroy(&cb);
  return true;
}

bool OverwriteFirstElement() {
  CircularBuffer cb;
  CircularBufferInit(&cb, sizeof(int), 2, NULL, NULL);
  int x = 1;
  CircularBufferPush(&cb, &x);
  x = 2;
  CircularBufferPush(&cb, &x);
  x = 3;
  CircularBufferPush(&cb, &x);
  CircularBufferPop(&cb, &x);
  ASSERT_INT_EQ(2, x);
  CircularBufferPop(&cb, &x);
  ASSERT_INT_EQ(3, x);
  CircularBufferDestroy(&cb);
  return true;
}

int CmpInt(void *a, void* b) {
  return *(int*)a - *(int*)b;
}

bool OverwriteMultipleTimes() {
  CircularBuffer cb;
  CircularBufferInit(&cb, sizeof(int), 10, NULL, CmpInt);
  for (int i = 0; i < 100; ++i) {
    CircularBufferPush(&cb, &i);
  }
  for (int i = 0; i < 90; ++i) {
    ASSERT_INT_EQ(0, CircularBufferCount(&cb, &i));
  }
  for (int i = 90; i < 100; ++i) {
    ASSERT_INT_EQ(1, CircularBufferCount(&cb, &i));
  }
  for (int i = 90; i < 100; ++i) {
    int k;
    CircularBufferPop(&cb, &k);
    ASSERT_INT_EQ(i, k);
  }
  CircularBufferDestroy(&cb);
  return true;
}

// CircularBuffer with strings

void FreeStr(void* ptr) {
  free(*(char**)ptr);
}

int CmpStr(void* a, void* b) {
  return strcmp(*(char**)a, *(char**)b);
}

bool Strings() {
  CircularBuffer cb;
  CircularBufferInit(&cb, sizeof(char**), 5, FreeStr, CmpStr);
  char* x;
  char* y;
  x = strdup("100");
  CircularBufferPush(&cb, &x);
  x = strdup("101");
  CircularBufferPush(&cb, &x);
  x = strdup("102");
  CircularBufferPush(&cb, &x);
  y = "101";
  ASSERT_INT_EQ(1, CircularBufferCount(&cb, &y));
  x = strdup("101");
  CircularBufferPush(&cb, &x);
  ASSERT_INT_EQ(2, CircularBufferCount(&cb, &y));
  CircularBufferPop(&cb, &y);
  ASSERT_STR_EQ("100", y);
  free(y);
  CircularBufferPop(&cb, &y);
  ASSERT_STR_EQ("101", y);
  free(y);
  y = "101";
  ASSERT_INT_EQ(1, CircularBufferCount(&cb, &y));
  CircularBufferDestroy(&cb);
  return true;
}

// StudentBuffer

bool CountStudents() {
  StudentBuffer sb;
  StudentBufferInit(&sb, 3);
  Student x, y, z, w;
  x.name = strdup("free");
  x.gpa = 4;
  y.name = strdup("uni");
  y.gpa = 3;
  z.name = strdup("free");
  z.gpa = 4;
  w.name = strdup("unknown");
  w.gpa = 100;
  StudentBufferPush(&sb, &x);
  StudentBufferPush(&sb, &y);
  StudentBufferPush(&sb, &z);
  ASSERT_INT_EQ(2, StudentBufferCount(&sb, &x));
  ASSERT_INT_EQ(1, StudentBufferCount(&sb, &y));
  ASSERT_INT_EQ(0, StudentBufferCount(&sb, &w));
  StudentBufferDestroy(&sb);
  return true;
}

int main(int argc, char** argv) {
  TEST(Basic);
  TEST(OverwriteFirstElement);
  TEST(OverwriteMultipleTimes);
  TEST(Strings);
  TEST(CountStudents);
  return 0;
}
