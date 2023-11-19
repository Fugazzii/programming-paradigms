#include "../ip_list.h"
#include "../packed_list.h"

#include <string.h>

#include "gl/lunit/lunit.h"

// PackedList

TEST(PL_Empty) {
  PackedList list;
  PackedListInit(&list, /*free_fn=*/NULL);
  ASSERT_INT_EQ(list.num_elems, 0);
  PackedListDispose(&list);
}

TEST(PL_Ints_Basic) {
  PackedList list;
  PackedListInit(&list, /*free_fn=*/NULL);
  int one = 1, two = 2, three = 3;
  PackedListAppend(&list, &one, sizeof(int));
  ASSERT_INT_EQ(list.num_elems, 1);
  PackedListAppend(&list, &two, sizeof(int));
  ASSERT_INT_EQ(list.num_elems, 2);
  PackedListAppend(&list, &three, sizeof(int));
  ASSERT_INT_EQ(list.num_elems, 3);
  ASSERT_INT_EQ(*(int*)PackedListGet(&list, 0), 1);
  ASSERT_INT_EQ(*(int*)PackedListGet(&list, 1), 2);
  ASSERT_INT_EQ(*(int*)PackedListGet(&list, 2), 3);
  PackedListDispose(&list);
}

int CmpInts(const void* a, const void* b) {
  return *(int*)a - *(int*)b;
}

TEST(PL_Ints_Sort) {
  PackedList list;
  PackedListInit(&list, /*free_fn=*/NULL);
  int one = 1, two = 2, three = 3, four = 4;
  PackedListAppend(&list, &three, sizeof(int));
  PackedListAppend(&list, &four, sizeof(int));
  PackedListAppend(&list, &two, sizeof(int));
  PackedListAppend(&list, &one, sizeof(int));
  ASSERT_INT_EQ(*(int*)PackedListGet(&list, 0), 3);
  ASSERT_INT_EQ(*(int*)PackedListGet(&list, 1), 4);
  ASSERT_INT_EQ(*(int*)PackedListGet(&list, 2), 2);
  ASSERT_INT_EQ(*(int*)PackedListGet(&list, 3), 1);  
  PackedListSort(&list, CmpInts);
  ASSERT_INT_EQ(*(int*)PackedListGet(&list, 0), 1);
  ASSERT_INT_EQ(*(int*)PackedListGet(&list, 1), 2);
  ASSERT_INT_EQ(*(int*)PackedListGet(&list, 2), 3);
  ASSERT_INT_EQ(*(int*)PackedListGet(&list, 3), 4);
  PackedListDispose(&list);
}

TEST(PL_CStrings) {
  PackedList list;
  PackedListInit(&list, /*free_fn=*/NULL);
  char* foo = "foo";
  char* bar = "bar";
  PackedListAppend(&list, foo, sizeof(char) * (strlen(foo) + 1));
  PackedListAppend(&list, bar, sizeof(char) * (strlen(bar) + 1));
  ASSERT_INT_EQ(list.num_elems, 2);
  ASSERT_STR_EQ((char*)PackedListGet(&list, 0), "foo");
  ASSERT_STR_EQ((char*)PackedListGet(&list, 1), "bar");  
  PackedListDispose(&list);  
}

int StrCmp(const void* a, const void* b) {
  return strcmp(a, b);
}

TEST(PL_CStrings_Sort) {
  PackedList list;
  PackedListInit(&list, /*free_fn=*/NULL);
  char* foo = "foo";
  char* bar = "bar";
  PackedListAppend(&list, foo, sizeof(char) * (strlen(foo) + 1));
  PackedListAppend(&list, bar, sizeof(char) * (strlen(bar) + 1));
  ASSERT_INT_EQ(list.num_elems, 2);
  ASSERT_STR_EQ((char*)PackedListGet(&list, 0), "foo");
  ASSERT_STR_EQ((char*)PackedListGet(&list, 1), "bar");
  PackedListSort(&list, StrCmp);
  ASSERT_INT_EQ(list.num_elems, 2);
  ASSERT_STR_EQ((char*)PackedListGet(&list, 0), "bar");
  ASSERT_STR_EQ((char*)PackedListGet(&list, 1), "foo");
  PackedListDispose(&list);  
}

// IPList

TEST(IP_Empty) {
  IPList list;
  IPListInit(&list);
  ASSERT_INT_EQ(list.num_elems, 0);
  IPListDispose(&list);
}

TEST(IP_IPv4) {
  IPList list;
  IPListInit(&list);
  IPv4 a, b;
  IPv4Init(&a, 127, 0, 0, 1);
  IPv4Init(&b, 127, 0, 0, 2);
  IPListAppendV4(&list, &a);
  IPListAppendV4(&list, &b);
  ASSERT_INT_EQ(list.num_elems, 2);
  const IPv4* x = IPListGet(&list, 0);
  ASSERT_INT_EQ(x->version, V4);
  ASSERT_INT_EQ(x->address[0], 127);
  ASSERT_INT_EQ(x->address[1], 0);
  ASSERT_INT_EQ(x->address[2], 0);
  ASSERT_INT_EQ(x->address[3], 1);
  const IPv4* y = IPListGet(&list, 1);
  ASSERT_INT_EQ(y->version, V4);
  ASSERT_INT_EQ(y->address[0], 127);
  ASSERT_INT_EQ(y->address[1], 0);
  ASSERT_INT_EQ(y->address[2], 0);
  ASSERT_INT_EQ(y->address[3], 2);
  IPListDispose(&list);
}

TEST(IP_IPv4_Sort) {
  IPList list;
  IPListInit(&list);
  IPv4 a, b;
  IPv4Init(&a, 127, 0, 3, 1);
  IPv4Init(&b, 127, 0, 2, 1);
  IPListAppendV4(&list, &a);
  IPListAppendV4(&list, &b);
  ASSERT_INT_EQ(list.num_elems, 2);
  const IPv4* x = IPListGet(&list, 0);
  ASSERT_INT_EQ(x->version, V4);
  ASSERT_INT_EQ(x->address[0], 127);
  ASSERT_INT_EQ(x->address[1], 0);
  ASSERT_INT_EQ(x->address[2], 3);
  ASSERT_INT_EQ(x->address[3], 1);
  const IPv4* y = IPListGet(&list, 1);
  ASSERT_INT_EQ(y->version, V4);
  ASSERT_INT_EQ(y->address[0], 127);
  ASSERT_INT_EQ(y->address[1], 0);
  ASSERT_INT_EQ(y->address[2], 2);
  ASSERT_INT_EQ(y->address[3], 1);
  IPListSort(&list);
  x = IPListGet(&list, 0);
  ASSERT_INT_EQ(x->version, V4);
  ASSERT_INT_EQ(x->address[0], 127);
  ASSERT_INT_EQ(x->address[1], 0);
  ASSERT_INT_EQ(x->address[2], 2);  // NOTE
  ASSERT_INT_EQ(x->address[3], 1);
  y = IPListGet(&list, 1);
  ASSERT_INT_EQ(y->version, V4);
  ASSERT_INT_EQ(y->address[0], 127);
  ASSERT_INT_EQ(y->address[1], 0);
  ASSERT_INT_EQ(y->address[2], 3);  // NOTE
  ASSERT_INT_EQ(y->address[3], 1);
  IPListDispose(&list);
}

TEST(IP_IPv6) {
  IPList list;
  IPListInit(&list);
  IPv6 a, b;
  IPv6Init(&a, "foo");
  IPv6Init(&b, "bar");
  IPListAppendV6(&list, &a);
  IPListAppendV6(&list, &b);
  ASSERT_INT_EQ(list.num_elems, 2);
  const IPv6* x = IPListGet(&list, 0);
  ASSERT_INT_EQ(x->version, V6);
  ASSERT_STR_EQ(x->address, "foo");
  const IPv6* y = IPListGet(&list, 1);
  ASSERT_INT_EQ(y->version, V6);
  ASSERT_STR_EQ(y->address, "bar");
  IPListDispose(&list);
}

TEST(IP_IPv6_Sort) {
  IPList list;
  IPListInit(&list);
  IPv6 a, b;
  IPv6Init(&a, "foo");
  IPv6Init(&b, "bar");
  IPListAppendV6(&list, &a);
  IPListAppendV6(&list, &b);
  ASSERT_INT_EQ(list.num_elems, 2);
  const IPv6* x = IPListGet(&list, 0);
  ASSERT_INT_EQ(x->version, V6);
  ASSERT_STR_EQ(x->address, "foo");
  const IPv6* y = IPListGet(&list, 1);
  ASSERT_INT_EQ(y->version, V6);
  ASSERT_STR_EQ(y->address, "bar");
  IPListSort(&list);
  x = IPListGet(&list, 0);
  ASSERT_INT_EQ(x->version, V6);
  ASSERT_STR_EQ(x->address, "bar");
  y = IPListGet(&list, 1);
  ASSERT_INT_EQ(y->version, V6);
  ASSERT_STR_EQ(y->address, "foo");
  IPListDispose(&list);
}

TEST(IP_IPvMix) {
  IPList list;
  IPListInit(&list);
  // Init IPs
  IPv4 a, b;
  IPv4Init(&a, 127, 0, 0, 1);
  IPv4Init(&b, 127, 0, 0, 2);
  IPv6 c, d;
  IPv6Init(&c, "foo");
  IPv6Init(&d, "bar");
  // Append to list
  IPListAppendV4(&list, &a);
  IPListAppendV6(&list, &c);
  IPListAppendV4(&list, &b);
  IPListAppendV6(&list, &d);
  // Asserts
  ASSERT_INT_EQ(list.num_elems, 4);
  const IPv4* x = IPListGet(&list, 0);
  const IPv4* y = IPListGet(&list, 2);
  const IPv6* z = IPListGet(&list, 1);
  const IPv6* w = IPListGet(&list, 3);
  // x
  ASSERT_INT_EQ(x->version, V4);
  ASSERT_INT_EQ(x->address[0], 127);
  ASSERT_INT_EQ(x->address[1], 0);
  ASSERT_INT_EQ(x->address[2], 0);
  ASSERT_INT_EQ(x->address[3], 1);
  // y
  ASSERT_INT_EQ(y->version, V4);
  ASSERT_INT_EQ(y->address[0], 127);
  ASSERT_INT_EQ(y->address[1], 0);
  ASSERT_INT_EQ(y->address[2], 0);
  ASSERT_INT_EQ(y->address[3], 2);
  // z
  ASSERT_INT_EQ(z->version, V6);
  ASSERT_STR_EQ(z->address, "foo");
  // w
  ASSERT_INT_EQ(w->version, V6);
  ASSERT_STR_EQ(w->address, "bar");
  IPListDispose(&list);
}

TEST(IP_IPvMix_Sort) {
  IPList list;
  IPListInit(&list);
  // Init IPs
  IPv4 a, b;
  IPv4Init(&a, 127, 0, 5, 1);
  IPv4Init(&b, 127, 0, 2, 1);
  IPv6 c, d;
  IPv6Init(&c, "foo");
  IPv6Init(&d, "bar");
  // Append to list
  IPListAppendV4(&list, &a);
  IPListAppendV6(&list, &c);
  IPListAppendV4(&list, &b);
  IPListAppendV6(&list, &d);
  // Asserts
  ASSERT_INT_EQ(list.num_elems, 4);
  const IPv4* x = IPListGet(&list, 0);
  const IPv4* y = IPListGet(&list, 2);
  const IPv6* z = IPListGet(&list, 1);
  const IPv6* w = IPListGet(&list, 3);
  // x
  ASSERT_INT_EQ(x->version, V4);
  ASSERT_INT_EQ(x->address[0], 127);
  ASSERT_INT_EQ(x->address[1], 0);
  ASSERT_INT_EQ(x->address[2], 5);
  ASSERT_INT_EQ(x->address[3], 1);
  // y
  ASSERT_INT_EQ(y->version, V4);
  ASSERT_INT_EQ(y->address[0], 127);
  ASSERT_INT_EQ(y->address[1], 0);
  ASSERT_INT_EQ(y->address[2], 2);
  ASSERT_INT_EQ(y->address[3], 1);
  // z
  ASSERT_INT_EQ(z->version, V6);
  ASSERT_STR_EQ(z->address, "foo");
  // w
  ASSERT_INT_EQ(w->version, V6);
  ASSERT_STR_EQ(w->address, "bar");
  // Sort
  IPListSort(&list);
  // Asserts after sort
  ASSERT_INT_EQ(list.num_elems, 4);
  x = IPListGet(&list, 0);
  y = IPListGet(&list, 1);
  z = IPListGet(&list, 2);
  w = IPListGet(&list, 3);
  // x
  ASSERT_INT_EQ(x->version, V4);
  ASSERT_INT_EQ(x->address[0], 127);
  ASSERT_INT_EQ(x->address[1], 0);
  ASSERT_INT_EQ(x->address[2], 2);
  ASSERT_INT_EQ(x->address[3], 1);
  // y
  ASSERT_INT_EQ(y->version, V4);
  ASSERT_INT_EQ(y->address[0], 127);
  ASSERT_INT_EQ(y->address[1], 0);
  ASSERT_INT_EQ(y->address[2], 5);
  ASSERT_INT_EQ(y->address[3], 1);
  // z
  ASSERT_INT_EQ(z->version, V6);
  ASSERT_STR_EQ(z->address, "bar");
  // w
  ASSERT_INT_EQ(w->version, V6);
  ASSERT_STR_EQ(w->address, "foo");
  IPListDispose(&list);
}

int main(int argc, char* argv[]) {
  LUnitOpts opts;
  LUnitOptsInit(&opts, argc, argv);

  TEST_SUITE_WITH_WEIGHT(all, 1.0);
  
  ADD_TEST(all, PL_Empty);
  ADD_TEST(all, PL_Ints_Basic);
  ADD_TEST(all, PL_Ints_Sort);
  ADD_TEST(all, PL_CStrings);
  ADD_TEST(all, PL_CStrings_Sort);
  ADD_TEST(all, IP_Empty);
  ADD_TEST(all, IP_IPv4);
  ADD_TEST(all, IP_IPv4_Sort);
  ADD_TEST(all, IP_IPv6);
  ADD_TEST(all, IP_IPv6_Sort);
  ADD_TEST(all, IP_IPvMix);
  ADD_TEST(all, IP_IPvMix_Sort);

  TestSuite* suites[] = {&all};
  ProcessTestSuites(1, suites, &opts);
  TestSuiteDispose(&all);
  return 0;
}
