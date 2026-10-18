#include <stdlib.h>
#include <stdio.h>
#include "array.h"

// define a matrix
// 	2d list
// 		nested array
// 			array
// 				resizable??
//

int main(void) {
	Array *a = New(sizeof(int));
	int num = 100;
	for (int i = 0; i < num; ++i) {
		printf("insert %d\n", i);
		Append(a, &i);
	}
	for (int i = 0; i < num; ++i) {;
		printf("Array at %d = %d\n", i, *((int *)Get(a, i)));
	}
	
	Delete(a);		
	printf("Hello World\n");
	return 0;
}
