#include <stdio.h>
#include <stdlib.h>
#include "memory-manager.h"

int* a[1000];
float* b[10][10];

void allocate_mult(){
	for (int i=0; i<1000; i++){
		n_alloc((void**)&a[i], 100*sizeof(int));
	}
}

void free_mult(){
	for (int i=0; i<1000; i++){
		n_free((void*)a[i]);
	}
}

void alloc_2dim() {
	for (int i = 0; i < 10; i++) {
		for(int j = 0; j < 10; j++) {
            n_alloc((void**)&b[i][j], (i+j)*sizeof(double));
		}
	}
}

void free_2dim() {
    for (int i = 0; i < 10; i++) {
		for(int j = 0; j < 10; j++) {
            n_free((void*)b[i][j]);
		}
	}
}

int main(int argc, char** argv){

	int* arr;
	int bb=10;
	n_alloc((void**)&arr, 10*sizeof(int));
	*(int*)(n_access_ptr(arr, arr))=bb;
	bb=0;
	bb=*(int*)(n_access_ptr(arr, arr));
	printf("%d\n", bb);
	n_free(arr);
	allocate_mult();
	free_mult();

    alloc_2dim();
    free_2dim();

	exit(0);
}
