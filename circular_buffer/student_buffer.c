#include "student_buffer.h"

#include <stdlib.h>
#include <string.h>

#include "circular_buffer.h"

void FreeStudent(void* ptr) {
  Student* student = ptr;
  free(student->name);
}

int cmp(int a, int b) {
  if(a > b) return 1;
  if(b > a) return -1;
  return 0;
}

int CmpStudent(void* a, void* b) {
  Student* s1 = a;
  Student* s2 = b;

  int nameCmpRes = strcmp(s1->name, s2->name);
  int gpaCmpRes = cmp(s1->gpa, s2->gpa);

  if(strcmp(s1->name, s2->name) == 0 && gpaCmpRes == 0) {
    return 0;
  }

  if(gpaCmpRes == 1) return 1;
  return -1;
}

// Do not change below this

void StudentBufferInit(StudentBuffer* sb, int capacity) {
  CircularBufferInit(sb, sizeof(Student), capacity, FreeStudent, CmpStudent);
}

void StudentBufferDestroy(StudentBuffer* sb) {
  CircularBufferDestroy(sb);
}

void StudentBufferPush(StudentBuffer* sb, Student* student) {
  CircularBufferPush(sb, student);
}

void StudentBufferPop(StudentBuffer* sb, Student* student) {
  CircularBufferPop(sb, student);
}

int StudentBufferCount(StudentBuffer* sb, Student* student) {
  return CircularBufferCount(sb, student);
}

