// Do not change this

#ifndef PARADIGMS_PROBLEMS_CIRCULAR_BUFFER_STUDENT_BUFFER_H_
#define PARADIGMS_PROBLEMS_CIRCULAR_BUFFER_STUDENT_BUFFER_H_

#include "circular_buffer.h"

typedef struct {
  char* name;
  short gpa;
} Student;

typedef CircularBuffer StudentBuffer;

void StudentBufferInit(StudentBuffer* sb, int capacity);
void StudentBufferDestroy(StudentBuffer* sb);
void StudentBufferPush(StudentBuffer* sb, Student* student);
void StudentBufferPop(StudentBuffer* sb, Student* student);
int StudentBufferCount(CircularBuffer* cb, Student* student);

#endif // PARADIGMS_PROBLEMS_CIRCULAR_BUFFER_STUDENT_BUFFER_H_
