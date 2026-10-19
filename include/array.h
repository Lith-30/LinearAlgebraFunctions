#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef ARRAY_H
#define ARRAY_H

typedef struct Array Array;

Array *NewArray(size_t elem_size); 

bool DeleteArray(Array *a); 

bool Replace(Array *a, uint64_t idx, void *val);

bool Resize(Array *a, uint64_t size);

bool Append(Array *a, void *val);

void *Get(Array *a, uint64_t idx);

bool Insert(Array *a, uint64_t idx, void *val);

bool Remove(Array *a, uint64_t idx);

bool Exists(Array *a, void *val);

uint64_t Count(Array *a, void *val);

#endif	// ARRAY_H

