#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "array.h"
#include "perf/perf.h"

// define a matrix
// 	2d list
// 		nested array
// 			array
// 				resizable??
//

int main(void) {
	Timer *t = NewTimer("Insert Timer");
	Array *a = NewArray(sizeof(uint8_t));
	
	int num = 999;
	Lap(t);
	for (int i = 0; i < num; ++i) {
		// printf("insert %d\n", i);
		Append(a, &i);
		// Lap(t);
	}
	Lap(t);
	for (int i = 0; i < num; ++i) {;
		// printf("Array at %d = %d\n", i, *((int *)Get(a, i)));
		Get(a, i);
		// Lap(t);
	}
	Lap(t);
	for (int i = 0; i < num; ++i) {
		int v = num - i;
		Replace(a, i, &v);
		// Lap(t);
	}
	Lap(t);
	// for (int i = 0; i < num; ++i) {;
	// 	printf("Array at %d = %d\n", i, *((int *)Get(a, i)));
	// 	// Get(a, i);
	// 	// Lap(t);
	// }
	
	DeleteArray(a);
	DeleteTimer(t);
	printf("Hello World\n");
	return 0;
}
