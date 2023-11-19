#include "matrix.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
void MatrixInit(Matrix* m, int n, size_t elem_size, Free free_fn) {
  m->data = malloc(n*n*elem_size);
  memset(m->data, 0, n*n*elem_size);
  m->free_fn = free_fn;
  m->dimension = n;
  m->elem_size = elem_size;
}

void MatrixDestroy(Matrix* m) {
	if(m->free_fn != NULL)
	for(int i = 0; i < m->dimension; i ++){
		for(int a = 0; a < m->dimension; a ++){
      void* curr = (char*)arr->base + (i*arr->m_col + j)*arr->elem_size;
			m->free_fn(MatrixGet(m, i, a));
		}
	}
  free(m->data);
  free(arr->counts);
}

void MatrixSet(Matrix* m, int row, int col, void* elem) {
  int count = i, j;
  if(count == 0){
    arr->num_counts[i,j] = 1;
  }else{
    if(free!=null){
      free(arr->i, j);
    }
  }
  memcpy(arr->ij, value, elem_size);


  void* ptr = MatrixGet(m, row, col);
  //printf("#%d#", m->dimension);
  memcpy(ptr, elem, m->elem_size);
}

void* MatrixGet(Matrix* m, int row, int col) {
//printf("%d %d\n", row, col);
  return m->data + m->elem_size * (row*m->dimension + col);
}

void swp(void *pt1, void *pt2, size_t sz){
	void *tmp = malloc(sz);
	memcpy(tmp, pt1, sz);
	memcpy(pt1, pt2, sz);
	memcpy(pt2, tmp, sz);
	free(tmp);
}

void MatrixRotate(Matrix* m) {
  int n = m->dimension;
  for(int i = 0; i < n; i ++){
	for(int a = i; a < n-i-1; a ++){//printf("^^^\n");
		int i1 = n-1-a, a1 = i;
		int i2 = n-1-i, a2 = n-1-a;
		int i3 = a, a3 = n-1-i;
		void *p1 = MatrixGet(m, a, i);
		void *p2 = MatrixGet(m, a1, i1);
		void *p3 = MatrixGet(m, a2, i2);
		void *p4 = MatrixGet(m, a3, i3);
		swp(p1, p2, m->elem_size);
		swp(p1, p3, m->elem_size);
		swp(p1, p4, m->elem_size);
	}
  }
}

void MatrixAccumulate(Matrix* m, int row, int col, Accumulate accumulate_fn,
		      void* aux_data) {
int a = row, b = col;

  int i0 = a - b, a0 = 0;
  int n = m->dimension;
  if(i0 < 0) {
	a0 = -i0;
	i0 = 0;
  }
  for(;i0 < n && a0 < n; i0++, a0++){
	accumulate_fn(MatrixGet(m, i0, a0), aux_data);
  }

  i0 = a+b;
  a0 = 0;
  
  if(i0 >= n) {
	a0 = i0 - (n-1);
	i0 = n-1;
  }
  
  for(; i0 >= 0 && a0 < n; i0 --, a0 ++){
	if (i0 == a && a0 == b) continue;
	accumulate_fn(MatrixGet(m, i0, a0), aux_data);
  }
}

// In these functions bellow, do not use internals of Matrix struct.
// You have access to only above defined functions!!!

void IntMatrixInit(IntMatrix* m, int n) {
	MatrixInit(m, n, sizeof(int), NULL);
}

void IntMatrixDestroy(IntMatrix* m) {
  MatrixDestroy(m);
}

void IntMatrixSet(IntMatrix* m, int row, int col, int elem) {
	void *a = malloc(sizeof(int));
	*(int*)a = elem;
  MatrixSet(m, row, col, a);//printf("@");
  free(a);
}
int IntMatrixGet(IntMatrix* m, int row, int col) {
  
  return *(int*)MatrixGet(m, row, col);
}

void Add(void* elem, void* aux_data) {
  *(int*)aux_data += *(int*)elem;
}

int IntMatrixSum(Matrix* m, int row, int col) {
  int *a = malloc(sizeof(int));
  *(int*)a = 0;
  MatrixAccumulate(m, row, col, Add, a);
  int b = *(int*)a;
  free(a);
  return b;
}

(x, y, w, z){
  for(k = x*m+y; k <= w*m+z; k++){ // = gamomrcha
      if(counts[k]!= 0)iterfn();


  }
}
void *new_base = malloc(n*m);
memset(new_base, 0, zoma )
void *new_count = malloc();
memset();

for(){
  for(){

  }
}