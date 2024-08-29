// Created by Lithira Mahagoda this is the matrix ADT, all basix matrix operations such as; getting elements, rows, columns etc are done here
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <error.h>

typedef struct {
	int rows;
	int cols;
	int **matrix;
} Matrix;

typedef Matrix* M;

// create a new n (rows) by m (columns) matrix
M newMatrix(int n, int m) {
	M m = malloc(sizeof(Matrix));
	m->rows = n;
	m->cols = m;
	m->matrix = malloc(sizeof(int *) * n);
	for (int i = 0; i < n; i++) {
		m->matrix[i] = malloc(sizeof(int) * m);
	}

	return m;
}

// updates the matrix to be its own transpose
M transposeMatrix(M m) {
	// matrix is a square matrix transpose is ez
	if (m->rows == m->cols) {
		int half = m->row / 2;
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
				newM->matrix[i][j] = m->[j][i];
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

	return m->[i - 1][j - 1];
}

// returns the requested row in a array
int *getRow(M m, int row) {
	if (row < 1 || row > m->rows) {
		fprintf(stderr, "Requested matrix row out of bounds");
		exit(EXIT_FAILURE);
	}

	int *row = malloc(sizeof(int) * m->rows);
	for (int j = 0; j < m->cols; j++) {
		row[j] = m->matrix[row][j];
	}

	return row;
}

// returns the requested column in a array
int *getRow(M m, int col) {
	if (col < 1 || row > m->cols) {
		fprintf(stderr, "Requested matrix col out of bounds");
		exit(EXIT_FAILURE);
	}

	int *row = malloc(sizeof(int) * m->cols);
	for (int i = 0; i < m->rows; i++) {
		row[i] = m->matrix[i][col];
	}

	return row;
}



// in progress
int determinant(M m) {
	if (m->rows != m->cols) {
		fprintf(stderr, "Determinant is not defined for non-square matrices");
		exit(EXIT_FAILURE);
	}


}

//free matrix to conserve memory
void freeMatrix(M m) {
	for (int i = 0; i < m->rows; i++) {
		free(m->matrix[i]);
	}
	free(matrix);
	free(m);

	return;
}

