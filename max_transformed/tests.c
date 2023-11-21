#include "max_transformed.h"

#include <stdio.h>
#include <stdbool.h>

#define ASSERT_INT_EQ(a, b) {						\
    int expected = (a);							\
    int actual = (b);							\
    if (expected != actual) {						\
      printf("%s:%d Assertion failed: expected %d got %d\n", __FILE__, __LINE__, expected, actual); \
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


bool One() {
    char k[] = { 0b00000001, 0b00000000 };
    ASSERT_INT_EQ(8, FindMaxTransformed(k));
    return true;
}

bool Sample() {
    char k[] = { 0b01001101, 0b00110010, 0b01110001, 0b00000000 };
    ASSERT_INT_EQ(296, FindMaxTransformed(k));
    return true;
}

bool AllOnes() {
    char k[] = { 0b11111111, 0b00000000 };
    ASSERT_INT_EQ(502, FindMaxTransformed(k));
    return true;
}

bool SixtyFour() {
    char k[] = { 0b01000000, 0b00000000 };
    ASSERT_INT_EQ(128, FindMaxTransformed(k));
    return true;
}

bool MultipleAllOnes() {
    char k[] = { 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b00000000 };
    ASSERT_INT_EQ(502, FindMaxTransformed(k));
    return true;
}

bool RandomNumbers2() {
    char k[] = { 0b01001110, 0b11111101, 0b10001010, 0b00010010, 0b10100101, 0b10000111, 0b01100000, 0b11110011, 0b00000000 };
    ASSERT_INT_EQ(488, FindMaxTransformed(k));
    return true;
}

bool RandomNumbers1() {
    char k[] = { 0b01001110, 0b01111101, 0b00001010, 0b00010010, 0b00100101, 0b00000111, 0b01100000, 0b01110011, 0b00000000 };
    ASSERT_INT_EQ(360, FindMaxTransformed(k));
    return true;
}

int main(int argc, char** argv) {
    TEST(One);
    TEST(Sample);
    TEST(AllOnes);
    TEST(MultipleAllOnes);
    TEST(SixtyFour);
    TEST(RandomNumbers1);
    TEST(RandomNumbers2);
    return 0;
}
