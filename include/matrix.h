#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "perf/perf.h"


typedef void (*Add)(void *, void *);
typedef void (*Mult)(void *, void *);
typedef struct Matrix Matrix;

Matrix *NewMatrix(uint64_t row, uint64_t col, size_t elem_size, Add add, Mult mult, void *zero);

bool DeleteMatrix(Matrix *m);

void *GetMatrix(Matrix *m, uint64_t row, uint64_t col);

bool ReplaceMatrix(Matrix *m, uint64_t row, uint64_t col, void *val);

bool AddMatrix(Matrix *m1, Matrix *m2);

Matrix *MultiplyMatrix(Matrix *m1, Matrix *m2);