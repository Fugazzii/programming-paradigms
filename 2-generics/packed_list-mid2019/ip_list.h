#ifndef PARADIGMS_PROBLEMS_PACKED_LIST_IP_LIST_H_
#define PARADIGMS_PROBLEMS_PACKED_LIST_IP_LIST_H_

#include "packed_list.h"

typedef enum {
	      V4 = 0,
	      V6 = 1
} IPVersion;

typedef struct {
  IPVersion version;
  char address[4];
} IPv4;

typedef struct {
  IPVersion version;
  char* address;
} IPv6;

// Initilizes IPv4.
void IPv4Init(IPv4* ip, char a, char b, char c, char d);

// Initilizes IPv4.
void IPv6Init(IPv6* ip, const char* address);

// Cleans up IPv6.
void IPv6Dispose(IPv6* ip);

typedef PackedList IPList;

void IPListInit(IPList* list);
void IPListDispose(IPList* list);
void IPListAppendV4(IPList* list, IPv4* v4);
void IPListAppendV6(IPList* list, IPv6* v6);
const void* IPListGet(IPList* list, int index);
// Sorts given list of IP addresses. V4 type IP addresses are smaller then V6,
// addresses of same version are sorted alphabetically.
void IPListSort(IPList* list);

#endif // PARADIGMS_PROBLEMS_PACKED_LIST_IP_LIST_H_
