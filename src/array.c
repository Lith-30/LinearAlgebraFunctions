#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "array.h"

#define BASE_LEN 1024


typedef struct Array {
	uint64_t size;		// total number of elements that can be stored
	uint64_t capacity;	// number of elements that are currently stored
	size_t elem_size;	// size of each individual element
	char *arr;		// raw array
} Array;

// Get the address of the array at index i
#define ADDR(a, i) ((a)->arr + ((i) * (a)->elem_size))

// Replace array element at index i with value v
#define REPLACE(a, i, v) (memcpy(ADDR(a, i), (v), (a)->elem_size))

bool InsideBounds_(Array *a, uint64_t idx) {
	return idx < a->size && idx >= 0;
}

Array *New(size_t elem_size) {
	Array *n = malloc(sizeof(Array));
	n->arr = malloc(elem_size * BASE_LEN);
	n->size = 0;
	n->capacity = BASE_LEN;
	n->elem_size = elem_size;
	return n;
}

bool Delete(Array *a) {
	free(a->arr);
	free(a);

	return true;
}

bool Replace(Array *a, uint64_t idx, void *val) {
	if (!InsideBounds_(a, idx)) {
		return 0;
	}
	//printf("Insert %ln, %d", &idx, *(int *)val);
	REPLACE(a, idx, val);
}

bool Resize(Array *a, uint64_t size) {
	if (a->size < size) {
		char *n_arr = realloc(a->arr, a->elem_size * size);
		if (n_arr == NULL) {
			return false;
		}
		a->arr = n_arr;
		a->capacity = size;
	}

	return true;
}

bool Append(Array *a, void *val) {
	if (a->size >= a->capacity) {
		if (!Resize(a, a->size * 2)) return false;
	}
	++a->size;
	
	Replace(a, a->size - 1, val);

	return true;
}

void *Get(Array *a, uint64_t idx) {
	if (!InsideBounds_(a, idx)) {
		// idk what to put here for now
		return 0;	
	}
	return ADDR(a, idx);
}

bool Insert(Array *a, uint64_t idx, void *val) {
	if (!InsideBounds_(a, idx)) {
		// idk what to put here for now
		return 0;
	}

	if (a->size >= a->capacity) {
		if (!Resize(a, a->size * 2)) return false;
	}

	for (int i = a->size; i >= idx + 1; --i) {
		REPLACE(a, i, ADDR(a, i - 1));
	}
	REPLACE(a, idx, val);
	++a->size;
	return true;
}

bool Remove(Array *a, uint64_t idx) {
	if (!InsideBounds_(a, idx)) return false;
	
	for (int i = idx; i < a->size; ++i) {
		REPLACE(a, i, ADDR(a, i + 1));
		
	}
	--a->size;
}

bool Exists(Array *a, void *val) {
	for (int i = 0; i < a->size; ++i) {
		if (memcmp(ADDR(a, i), val, a->elem_size)) return true;
	}

	return false;
}

uint64_t Count(Array *a, void *val) {
	uint64_t count = 0;	
	for (int i = 0; i < a->size; ++i) {
		if (memcmp(ADDR(a, i), val, a->elem_size)) ++count;
	}

	return count;
}


