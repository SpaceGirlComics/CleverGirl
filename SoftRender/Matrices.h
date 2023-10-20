#ifndef MATRICES_H
#define MATRICES_H

typedef struct _MATRIX
{
	float** mat;
	int rows;
	int cols;
} MATRIX, * LPMATRIX;

MATRIX createMatrix(int _rows, int _columns);
MATRIX copyMatrix(LPMATRIX _m);
MATRIX minorMatrix(LPMATRIX _m, int _x, int _y);
MATRIX cofactorMatrix(LPMATRIX _m);
MATRIX transposeMatrix(LPMATRIX _m);
MATRIX adjointMatrix(LPMATRIX _m);
MATRIX inverseMatrix(LPMATRIX _m);
MATRIX addMatrix(LPMATRIX _left, LPMATRIX _right);
MATRIX subtractMatrix(LPMATRIX _left, LPMATRIX _right);
MATRIX scaleMatrix(LPMATRIX _m, float _scalar);
MATRIX multiplyMatrix(LPMATRIX _left, LPMATRIX _right);

float det(LPMATRIX _m);
void destroyMatrix(MATRIX _m);

LPMATRIX createLPMatrix(int _rows, int _columns);
void destroyLPMatrix(LPMATRIX _m);

void translate(LPMATRIX _m, float _x, float _y, float _z);
void rotate(LPMATRIX _in, float _roll, float _pitch, float _yaw);
void scale(LPMATRIX _in, float _scalar);
void loadIdentity(LPMATRIX _m);

#endif
