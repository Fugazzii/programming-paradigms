#include "ip_list.h"

#include <string.h>
#include <stdlib.h>
#include "packed_list.h"
int cmp(const void* elem1,const void* elem2)
{
	const IPv4* tmp1=elem1;
	const IPv4* tmp2=elem2;
	if(tmp1->version==V4&&tmp2->version==V4)
	{
		for(int i=0;i<4;i++)
			if(tmp1->address[i]-tmp2->address[i]!=0)
				return tmp1->address[i]-tmp2->address[i];
		return 0;
	}
	if(tmp1->version==V6&&tmp2->version==V4)
	{
		return 1;
	}
	if(tmp1->version==V4&&tmp2->version==V6)
	{
		return -1;
	}
	const IPv6* tmp3=elem1;
	const IPv6* tmp4=elem2;
	return strcmp(tmp3->address,tmp4->address);
}
void IPv4Init(IPv4* ip, char a, char b, char c, char d) {
	ip->address[0]=a;
	ip->address[1]=b;
	ip->address[2]=c;
	ip->address[3]=d;
	ip->version=V4;
}

void IPv6Init(IPv6* ip, const char* address) {
	ip->version=V6;
	ip->address=malloc(strlen(address)+1);
	strcpy(ip->address,address);
}

void IPv6Dispose(IPv6* ip) {
	free(ip->address);
}

void IPListInit(IPList* list) {
	PackedListInit(list,NULL);
}

void IPListDispose(IPList* list) {
	for(int i=0;i<list->num_elems;i++)
	{
		IPv6* elem=(IPv6*)((char*)list->base+list->elem_offsets[i]);
		if(elem->version==V6)
			IPv6Dispose(elem);
	}
	PackedListDispose(list);
}

void IPListAppendV4(IPList* list, IPv4* v4) {
	PackedListAppend(list,v4,sizeof(IPv4));
}

void IPListAppendV6(IPList* list, IPv6* v6) {
	PackedListAppend(list,v6,sizeof(IPv6));
}

const void* IPListGet(IPList* list, int index) {
  return PackedListGet(list,index);
}

void IPListSort(IPList* list) {
	PackedListSort(list,cmp);
}
