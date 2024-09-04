// Created by Lithira Mahagoda this is the matrix ADT, all basix matrix operations such as; getting elements, rows, columns etc are done here
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>

#define MAX_DIGITS 10

typedef struct {
	int rows;
	int cols;
	float **matrix;
} Matrix;

typedef Matrix* M;

M newMatrix(int n, int m);
M transposeMatrix(M m);
int getElement(M m, int i, int j);
int *getRow(M m, int row);
int *getCol(M m, int col);
M multiplyMatrix(M m1, M m2);
M copyMatrix(M m);
void insertValue(M m, int i, int j, float val);
void rewriteMatrix(M m, float *vals, int num_vals);
void overwriteMatrix(M source, M sink);
void rowEchelonForm(M m);
void rowReduce(M m, int row1, int row2, float factor);
int determinant(M m);
void freeMatrix(M m);
void displayMatrix(M m);

int main(void) {
	return 0;
}

void displayMatrix(M m) {
	int longestNumDigits = 0;
	char buffer[MAX_DIGITS + 1];
	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->cols; j++) {
			gcvt(m->matrix[i][j], MAX_DIGITS + 1, buffer);
			int len = strlen(buffer);
			longestNumDigits = longestNumDigits * (longestNumDigits > len) + len * (len <= longestNumDigits);
		}
	}

	// so to leave at least one space between all elements we must have longestNumDigits + 1 space for each number
	for (int i = 0; i < m->rows; i++) {
		printf("| ");
		for (int j = 0; j < m->cols; j++) {
			gcvt(m->matrix[i][j], MAX_DIGITS + 1, buffer);
			int len = strlen(buffer);
			//display number
			printf("%s", buffer);
			// insert appropriate spacings
			for (int spaces = 0; spaces < longestNumDigits - len + 1; spaces++) {
				printf(" ");
			}
		}
		printf("|\n");
	}
	
	return;
}

// create a new n (rows) by m (columns) 0 matrix
M newMatrix(int n, int m) {
	M new = malloc(sizeof(Matrix));
	new->rows = n;
	new->cols = m;
	new->matrix = malloc(sizeof(float *) * n);
	for (int i = 0; i < n; i++) {
		new->matrix[i] = malloc(sizeof(float) * m);
		for (int j = 0; j < new->cols; j++) {
			new->matrix[i][j] = 0;
		}
	}

	return new;
}

// updates the matrix to be its own transpose
M transposeMatrix(M m) {
	// matrix is a square matrix transpose is ez
	if (m->rows == m->cols) {
		int half = m->rows / 2;
		for (int j = 0; j < m->cols; j++) {
			for (int i = 0; i <= j; i++) {
				int temp = m->matrix[i][j];
				m->matrix[i][j] = m->matrix[j][i];
				m->matrix[j][i] = temp;
			}
		}

	} else {
		// if matrix not square we return a new matrix
		M newM = newMatrix(m->cols, m->rows);
		for (int i = 0; i < m->cols; i++) {
			for (int j = 0; j < m->rows; j++) {
				newM->matrix[i][j] = m->matrix[j][i];
			}
		}
		freeMatrix(m);
		m = newM;
	}

	return m;
	
}
// get the ijth element of a matrix
int getElement(M m, int i, int j) {
	if (i > m->rows || j > m->cols || i < 1 || j < 1) {
		fprintf(stderr, "Requested matrix element out of bounds");
		exit(EXIT_FAILURE);
	}

	return m->matrix[i - 1][j - 1];
}

// returns the requested row in a array
int *getRow(M m, int row) {
	if (row < 1 || row > m->rows) {
		fprintf(stderr, "Requested matrix row out of bounds");
		exit(EXIT_FAILURE);
	}

	int *rows = malloc(sizeof(int) * m->rows);
	for (int j = 0; j < m->cols; j++) {
		rows[j] = m->matrix[row][j];
	}

	return rows;
}

// returns the requested column in a array
int *getCol(M m, int col) {
	if (col < 1 || col > m->cols) {
		fprintf(stderr, "Requested matrix col out of bounds");
		exit(EXIT_FAILURE);
	}

	int *cols = malloc(sizeof(int) * m->cols);
	for (int i = 0; i < m->rows; i++) {
		cols[i] = m->matrix[i][col];
	}

	return cols;
}

M multiplyMatrix(M m1, M m2) {
	if (m1->rows != m2->cols || m1->cols != m2->rows) {
		fprintf(stderr, "Matrices cannot be multiplied due to non-matching row and column sizes");
		exit(EXIT_FAILURE);
	}
	M result = newMatrix(m1->rows, m2->cols);
	
	for (int i = 0; i < result->rows; i++) {
		for (int j = 0; j < result->cols; j++) {
			int prodsum = 0;
			for (int k = 0; k < m1->cols; k++) {
				prodsum += m1->matrix[i][k] * m2->matrix[k][j];
			}
			result->matrix[i][j] = prodsum;
		}
	}

	return result;
}

// return a copy of a matrix
M copyMatrix(M m) {
	M new = newMatrix(m->rows, m->cols);
	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->cols; j++) {
			new->matrix[i][j] = m->matrix[i][j];
		}
	}

	return new;
}

// update or insert a new value into a given position in a matrix
void insertValue(M m, int i, int j, float val) {
	m->matrix[i - 1][j - 1] = val;
	return;
}

// update the first nm_vals values of a matrix
void rewriteMatrix(M m, float *vals, int num_vals) {
	for (int i = 0; i < num_vals; i++) {
		m->matrix[i / m->cols][i % m->cols] = vals[i];
	}

	return;
}

// copies all of the entries from source to sink
void overwriteMatrix(M source, M sink) {
	if (source->rows != sink->rows || source->cols != sink->cols) {
		fprintf(stderr, "matrix row and column sizes do not match");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < source->rows; i++) {
		for (int j = 0; j < source->cols; j++) {
			sink->matrix[i][j] = source->matrix[i][j];
		}
	}
}

void rowEchelonForm(M m) {
	M copy = copyMatrix(m);

	for (int j = 0; j < copy->cols; j++) {
		for (int i = j + 1; i < copy->rows; i++) {
			if (copy->matrix[i][j] != 0 && copy->matrix[j][j] != 0) {
				rowReduce(copy, j, i, -1 * m->matrix[i][j] / m->matrix[j][j]);	
			}
		}
	}

	return;
}

void rowReduce(M m, int row1, int row2, float factor) {
	// generate a matrix to perform the row reduction
	M red = newMatrix(m->rows, m->rows);
	red->matrix[row2][row1] = factor;
	// set new matrix to be en identity matrix
	for (int i = 0;  i < m->rows; i++) {
		red->matrix[i][i] = 1;
	}

	M result = multiplyMatrix(red, m);
	
	overwriteMatrix(result, m);

	freeMatrix(result);

	return;
}

// find the determinant of a matrix
int determinant(M m) {
	if (m->rows != m->cols) {
		fprintf(stderr, "Determinant is not defined for non-square matrices.");
		exit(EXIT_FAILURE);
	}
	// create a copy to get row reduced
	M copy = copyMatrix(m);

	rowEchelonForm(copy);
	
	int det = m->matrix[0][0];
	
	// multiply the diagonal
	for (int i = 1; i < m->rows; i++) {
		det *= m->matrix[i][i];
	}

	return det;
}

//free matrix to conserve memory
void freeMatrix(M m) {
	for (int i = 0; i < m->rows; i++) {
		free(m->matrix[i]);
	}
	free(m->matrix);
	free(m);

	return;
}

