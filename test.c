#include <stdio.h>
#include <stdlib.h>
#include "memory-manager.h"

int main(int argc, char** argv){

	int* arr;
	int bb=10;
	n_alloc((void**)&arr, 10*sizeof(int));
	*(int*)(n_access_ptr(arr, arr))=bb;
	bb=0;
	bb=*(int*)(n_access_ptr(arr, arr));
	printf("%d", bb);
	n_free(arr);

	exit(0);
}
