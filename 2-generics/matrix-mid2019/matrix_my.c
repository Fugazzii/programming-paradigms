#include "matrix.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
void MatrixInit(Matrix* m, int n, size_t elem_size, Free free_fn) {
    m->free_fn = free_fn;
    m->elem_size = elem_size;
    m->dimension = n;
    m->data = malloc(n*n*m->elem_size); //nxn matrix
    assert(m->data != NULL);
    memset(m->data, 0, n*n*m->elem_size);
}

void MatrixDestroy(Matrix* m) {
    if(m->free_fn != NULL){
        for(int i= 0; i < m->dimension; i ++){
            for(int j = 0; j < m->dimension; j++){
                m->free_fn(MatrixGet(m, i, j));
            }
        }
    }
    free(m->data);
}

void MatrixSet(Matrix* m, int row, int col, void* elem) {
    void *currAddr = (char *)m->data +  m->elem_size* (m->dimension*row + col);
    memcpy(currAddr, elem, m->elem_size);
}

void* MatrixGet(Matrix* m, int row, int col) {
    return ((char*)m->data + m->elem_size*(row*m->dimension + col));
}
void MatrixSwap(size_t el_size, void *ptr1, void *ptr2){
    void *tmp = malloc(el_size);
    //tmp = ptr1;
    //ptr1 = ptr2;
    //ptr2 = tmp;
    memcpy(tmp, ptr1, el_size);
    memcpy(ptr1, ptr2, el_size);
    memcpy(ptr2, tmp, el_size);
    free(tmp);
}
void MatrixRotate(Matrix* m) {
    int n = m->dimension;
    for(int i = 0; i < n; i ++){
        for(int j = 0; j < n; j++){

        }
    }



}

void MatrixAccumulate(Matrix* m, int row, int col, Accumulate accumulate_fn,
		      void* aux_data) {

    // row-1, col-1
    //(row-1)*m->dimension +(col-1);
    // row-1, col+1
    //(row-1)*m->dimension +(col+1);
    // row+1, col-1
    //(row+1)*m->dimension +(col-1);
    // row+1, col+1  
   // (row+1)*m->dimension +(col+1);


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
    MatrixSet(m, row, col, &elem);
}
int IntMatrixGet(IntMatrix* m, int row, int col) {
    int *getelem = (int*)MatrixGet(m, row, col);
    return *getelem;
}
void IntAccumulate(void* elem, void* aux_data){
    (*(int*)aux_data) += *(int*)elem;
}

int IntMatrixSum(Matrix* m, int row, int col) {
    void *aux_data = malloc(sizeof(int));
    MatrixAccumulate(m, row, col, IntAccumulate, aux_data);
    int result = *(int*)aux_data;
    free(aux_data);
    return result;
}
