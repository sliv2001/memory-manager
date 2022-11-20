#include <stdio.h>
#include "memory-manager.h"

int main(){

	int* arr;
	int bb=10;
	n_alloc((void**)&arr, 10*sizeof(int));
	n_write(arr, &bb, 0, sizeof(bb));
	bb=0;
	n_read(arr, &bb, 0, sizeof(bb));
	printf("%d", bb);
	n_free(arr);

	return 0;
}
