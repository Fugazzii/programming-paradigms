#include "student_buffer.h"

#include <stdlib.h>
#include <string.h>

#include "circular_buffer.h"

void FreeStudent(void* ptr) {

}

int CmpStudent(void* a, void* b) {

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
  CircularBufferPush(sb, student);
}

int StudentBufferCount(StudentBuffer* sb, Student* student) {
  return CircularBufferCount(sb, student);
}

