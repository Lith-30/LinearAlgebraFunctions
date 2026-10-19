#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef ARRAY_H
#define ARRAY_H

typedef struct Array Array;

Array *NewArray(size_t len, size_t elem_size); 

bool DeleteArray(Array *a); 

bool ReplaceArray(Array *a, uint64_t idx, void *val);

bool ResizeArray(Array *a, uint64_t size);

bool AppendArray(Array *a, void *val);

void *GetArray(Array *a, uint64_t idx);

bool InsertArray(Array *a, uint64_t idx, void *val);

bool RemoveArray(Array *a, uint64_t idx);

bool ExistsArray(Array *a, void *val);

uint64_t CountArray(Array *a, void *val);

#endif	// ARRAY_H

