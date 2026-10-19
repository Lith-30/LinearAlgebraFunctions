#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "array.h"

#define BASE_LEN 1024


typedef struct Array {
	uint64_t size;		// number of elements that are currently stored
	uint64_t capacity;	// total number of elements that can be stored
	size_t elem_size;	// size of each individual element
	char *arr;		// raw array
} Array;

// Get the address of the array at index i
#define ADDR(a, i) ((a)->arr + ((i) * (a)->elem_size))

// Replace array element at index i with value v
#define REPLACE(a, i, v) (memcpy(ADDR(a, i), (v), (a)->elem_size))

static inline bool InsideBounds_(Array *a, uint64_t idx) {
	return idx < a->size;
}

Array *NewArray(size_t elem_size) {
	Array *n = malloc(sizeof(Array));
	n->arr = malloc(elem_size * BASE_LEN);
	n->size = 0;
	n->capacity = BASE_LEN;
	n->elem_size = elem_size;
	return n;
}

bool DeleteArray(Array *a) {
	free(a->arr);
	free(a);

	return true;
}

bool Replace(Array *a, uint64_t idx, void *val) {
	if (!InsideBounds_(a, idx)) {
		return false;
	}
	REPLACE(a, idx, val);
	return true;
}

bool Resize(Array *a, uint64_t size) {
	if (a->capacity < size) {
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
		uint64_t new_cap = a->capacity == 0 ? BASE_LEN : a->capacity * 2;
		if (!Resize(a, new_cap)) return false;
	}
	++a->size;
	
	Replace(a, a->size - 1, val);

	return true;
}

void *Get(Array *a, uint64_t idx) {
	if (!InsideBounds_(a, idx)) {
		return NULL;
	}
	return ADDR(a, idx);
}

bool Insert(Array *a, uint64_t idx, void *val) {
	if (!InsideBounds_(a, idx)) {
		return false;
	}

	if (a->size >= a->capacity) {
		uint64_t new_cap = a->capacity == 0 ? BASE_LEN : a->capacity * 2;
		if (!Resize(a, new_cap)) return false;
	}

	// Move elements to the right to make space
	memmove(ADDR(a, idx + 1), ADDR(a, idx), (a->size - idx) * a->elem_size);
	REPLACE(a, idx, val);
	++a->size;
	return true;
}

bool Remove(Array *a, uint64_t idx) {
	if (!InsideBounds_(a, idx)) return false;
	
	// Move elements to the left to fill the gap
	memmove(ADDR(a, idx), ADDR(a, idx + 1), (a->size - idx - 1) * a->elem_size);
	--a->size;

	return true;
}

bool Exists(Array *a, void *val) {
	for (uint64_t i = 0; i < a->size; ++i) {
		if (memcmp(ADDR(a, i), val, a->elem_size) == 0) return true;
	}

	return false;
}

uint64_t Count(Array *a, void *val) {
	uint64_t count = 0;	
	for (uint64_t i = 0; i < a->size; ++i) {
		if (memcmp(ADDR(a, i), val, a->elem_size) == 0) ++count;
	}

	return count;
}