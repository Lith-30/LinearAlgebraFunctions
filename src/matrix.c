#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "array.h"
#include "perf/perf.h"
#include "matrix.h"

// define a matrix
// 	2d list
// 		nested array
// 			array
// 				resizable??
//

int main(void) {
	Timer *t = NewTimer("Insert Timer");
	Array *a = NewArray(0, sizeof(uint8_t));
	
	int num = 10000;
	// Lap(t);
	for (int i = 0; i < num; ++i) {
		// printf("insert %d\n", i);
		AppendArray(a, &i);
		// Lap(t);
	}
	// Lap(t);
	for (int i = 0; i < num; ++i) {;
		// printf("Array at %d = %d\n", i, *((int *)GetArray(a, i)));
		GetArray(a, i);
		// Lap(t);
	}
	
	bool real = false;
	Lap(t);
	for (int i = 0; i < num; ++i) {
		int v = num - i;
		ReplaceArray(a, i, &v);
		// Lap(t);
	}
	Lap(t);
	if (real) printf("Hello World\n");
	// for (int i = 0; i < num; ++i) {;
	// 	printf("Array at %d = %d\n", i, *((int *)GetArray(a, i)));
	// 	// Get(a, i);
	// 	// Lap(t);
	// }
	
	DeleteArray(a);
	DeleteTimer(t);
	
	return 0;
}

typedef void (*Add)(void *, void *);
typedef void (*Mult)(void *, void *);

typedef struct Matrix {
	uint64_t rows;		// Number of rows
	uint64_t cols;		// Number of cols
	uint64_t elem_size;
	// function pointers for elementwise operations
	Add add;
	Mult mult;

	void *zero;			// Define the zero value for the type that you are using
	Array *mat;			// Matrix array
} Matrix;

static inline bool InsideBoundsMatrix_(Matrix *m, uint64_t row, uint64_t col) {
	return (row < m->rows && col < m->cols);
}

Matrix *NewMatrix(uint64_t row, uint64_t col, size_t elem_size, Add add, Mult mult, void *zero) {
	if (row <= 0 || col <= 0) return NULL;

	Matrix *m = malloc(sizeof(Matrix));
	m->rows = row;
	m->cols = col;
	m->add = add;
	m->mult = mult;
	m->mat = NewArray(row, sizeof(Array *));
	m->zero = malloc(elem_size);
	m->elem_size = elem_size;

	memcpy(m->zero, zero, elem_size);
	for (int i = 0; i < row; i++) {
		AppendArray(m->mat, NewArray(col, elem_size));
	}
	return m;
}

bool DeleteMatrix(Matrix *m) {
	for (int i = 0; i < m->rows; ++i) {
		DeleteArray(GetArray(m->mat, i));
 	}
	DeleteArray(m->mat);

	free(m);

	return true;
}

void *GetMatrix(Matrix *m, uint64_t row, uint64_t col) {
	Array *r = GetArray(m->mat, row);
	if (!r) return 0;
	return GetArray(r, col);
}

bool ReplaceMatrix(Matrix *m, uint64_t row, uint64_t col, void *val) {
	Array *r = GetArray(m->mat, row);
	if (!r) return 0;
	return ReplaceArray(r, col, val);
}

/*
	Add should be a + operator for the matrix elements, up to user definition.
	Add is done inplace with the first parameter storing the result
	e.g. add function for uint8_t
	void add(void *l, void *r) {
		*((uint8_t *)l) = *((uint8_t *)l) + *((uint8_t *)r)
	}
*/
bool AddMatrix(Matrix *m1, Matrix *m2) {
	// Done inplace into matrix m1.
	if (m1->rows != m2->rows || m1->cols != m2->cols || m1->elem_size != m2->elem_size) return false;

	for (int r = 0; r < m1->rows; ++r) {
		for (int c = 0; c < m1->cols; ++c) {
			m1->add(GetMatrix(m1, r, c), GetMatrix(m2, r, c));
		}
	}
	return true;
}

Matrix *MultiplyMatrix(Matrix *m1, Matrix *m2) {
	if (m1->rows != m2->rows || m1->cols != m2->rows || m1->elem_size != m2->elem_size) return NULL;
	Matrix *res = NewMatrix(m2->cols, m1->rows, m1->elem_size, m1->add, m1->mult, m1->zero);
	
	// initalise a zero sum
	void *sum = malloc(m1->elem_size);
	memcpy(res, m1->zero, m1->elem_size);
	void *tmp = malloc(m1->elem_size);


	for (int n_c = 0; n_c < m1->cols; ++n_c) {
		for (int r = 0; r < m1->rows; ++r) {
			for (int c = 0; c < m2->cols; ++c) {
				// m1->mult(GetMatrix(m1, r, n_c), GetMatrix(m2, n_c, c));
				memcpy(tmp, GetMatrix(m1, r, n_c), m1->elem_size);
				m1->mult(tmp, GetMatrix(m2, n_c, c));
				m1->add(sum, tmp);
			}
			ReplaceMatrix(res, r, n_c, sum);
			memcpy(sum, m1->zero, m1->elem_size);
		}
	}
	free(sum);
	free(tmp);
	return res;
}

void PrintMatrix(Matrix *m, char *delim) {
	for (int i = 0; i < m->rows; ++i) {
		for (int j = 0; j < m->cols; ++j) {
			printf(delim, GetMatrix(m, r, c));
			printf(" ");
		}
		printf("\n");
	}
}